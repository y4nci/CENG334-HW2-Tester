//
// Created by Baran Yancı on 9.05.2023.
//

#include "TesterModule.h"

TesterModule::TesterModule() {
    std::cout << "TesterModule created." << std::endl;
}

TesterModule::TesterModule(Arguments arguments) {
    std::cout << "TesterModule created." << std::endl;
    this->arguments = arguments;
}

TesterModule::~TesterModule() {
    delete this->matrixGroups;

    system("rm -f logs/output*.txt");
}

void TesterModule::confirmExecutable(const char* executablePath) {
    char msg[] = "Executable is not found, or not executable.\n" \
                 "Please make sure this file is in the same directory with the executable called 'hw2'.\n";

    if (access(executablePath, X_OK) == -1) {
        std::cerr << msg << std::endl;
        exit(1);
    }
}

void TesterModule::initialiseEnvironment() {
    this->sumSyncDetected = false;
    this->mulSyncDetected = false;
    this->matrixGroups = generateRandomMatrixGroups(arguments.matrixGroupCount, arguments.minDimension, arguments.maxDimension);

    std::cout << "Environment initialised." << std::endl;
}

void TesterModule::logResult(const char* title, bool passed) {
    std::cout << title << ": " << (passed ? "PASSED" : "FAILED") << std::endl;
}

void TesterModule::run() {
    const char executablePath[] = "./hw2";

    std::cout << "Tester started." << std::endl;

    for (int i = 0; i < arguments.matrixGroupCount; i++) {
        char outputPath[24];
        pid_t pid;
        int fd[2];

        snprintf(outputPath, 24, "logs/output%d.txt", i+1);

        pipe(fd);

        pid = Fork();

        if (pid == 0) {
            // Child process
            int outputFile;
            outputFile = open(outputPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

            close(fd[1]);

            if (outputFile == -1) {
                exit(1);
            }

            dup2(outputFile, STDOUT_FILENO);
            dup2(fd[0], STDIN_FILENO);
            execl(executablePath, executablePath, NULL);
        } else if (pid > 0) {
            // Parent process

            int status, lastMatrixId = INT_MIN, row = 0;
            unsigned finalRow = matrixGroups->at(i).values[0].getMatrixValues().size()
                    , finalColumn = matrixGroups->at(i).values[3].getMatrixValues()[0].size()
                    , N = finalRow, K = finalColumn, M = matrixGroups->at(i).values[0].getMatrixValues()[0].size();
            FILE *outputFile;
            Matrix expectedSum12 = matrixGroups->at(i).values[0] + matrixGroups->at(i).values[1]
                    , expectedSum34 = matrixGroups->at(i).values[2] + matrixGroups->at(i).values[3]
                    , expectedFinal = expectedSum12 * expectedSum34
                    , actualSum12(N, M, 0), actualSum34(M, K, 0)

                    /* actualFinal_cellByCell is a matrix containing the values it read from the outputs of the threads.
                     * whereas actualFinal_output is a matrix containing the values it read from the end of the output file.
                     */
                    , actualFinal_cellByCell(N, K, 0), actualFinal_output(N, K, 0);
            std::vector<std::string> threadIds;

            close(fd[0]);

            for (int j = 0; j < 4; j++) {
                std::cout << "Input matrix " << j+1 << " for case #" << i+1 << "\n";
                std::cout << matrixGroups->at(i).values[j].toString() << std::endl;
                write(fd[1], matrixGroups->at(i).values[j].toString().c_str(), matrixGroups->at(i).values[j].toString().length());
            }

            waitpid(pid, &status, 0);

            if (!WIFEXITED(status)) {
                std::cerr << "Child process exited abnormally." << std::endl;
                this->logResult(SUM_TEST_1, false);
                this->logResult(SUM_TEST_2, false);
                this->logResult(MATCHING_OUTPUTS, false);
                this->logResult(FINAL_MATRIX, false);
                this->logResult(THREAD_COUNT, false);
                std::cout << "----------------------------------------" << std::endl;
                continue;
            }

            outputFile = fopen(outputPath, "r");

            if (outputFile == NULL) {
                std::cerr << "Output file could not be opened." << std::endl;
                exit(1);
            }

            // read the output file line by line.
            // if the line is a matrix, parse it and compare it with the expected result.
            // the matrix is stored in the last lines of the output file.
            // the other lines have the following format:
            //     t:      5063 | (T140499550033664) Matrix2 (4,3):531
            //     t:      time | id                 m. id   (x,y):value
            // the counts of such lines should be equal to the total number of cells in the matrices.

            while (!feof(outputFile) && row < finalRow) {
                char line[64];

                // DEBUG
                std::cout << "Reading line " << row << std::endl;
                // DEBUG

                fgets(line, 64, outputFile);

                if (line[0] == 't') {
                    char id[64]; // TODO: use this id to check whether a row is actually computed by the thread that is supposed to compute it.
                    int time, x, y, value, matrixId;

                    if (!parseThreadString(line, time, id, matrixId, x, y, value)) {
                        std::cerr << "Thread string could not be parsed." << std::endl;
                        exit(1);
                    }

                    if (matrixId == 0) {
                        actualSum12[x-1][y-1] = value;
                    } else if (matrixId == 1) {
                        actualSum34[x-1][y-1] = value;
                    } else if (matrixId == 2) {
                        actualFinal_cellByCell[x-1][y-1] = value;
                    }

                    if (lastMatrixId == 2 && matrixId != 2) {
                        this->mulSyncDetected = true;
                    } else if (lastMatrixId == 1 && matrixId == 0) {
                        this->sumSyncDetected = true;
                    } else {
                        lastMatrixId = matrixId;
                    }

                    threadIds.push_back(id);
                } else {
                    // parse final matrix
                    // row by row

                    int column = 0;

                    for (char *token = strtok(line, " "); token != NULL; token = strtok(NULL, " ")) {
                        actualFinal_output[row][column++] = atoi(token);
                    }

                    row++;
                }
            }

            std::cout << "\nMatrix group " << i+1 << " is tested.\n\n" << std::endl;

            std::cout << "Expected sum of matrix 1 and 2:\n" << expectedSum12.toString() << std::endl;
            std::cout << "Actual sum of matrix 1 and 2:\n" << actualSum12.toString() << std::endl;
            std::cout << "Expected sum of matrix 3 and 4:\n" << expectedSum34.toString() << std::endl;
            std::cout << "Actual sum of matrix 3 and 4:\n" << actualSum34.toString() << std::endl;
            std::cout << "Expected final matrix:\n" << expectedFinal.toString() << std::endl;
            std::cout << "Actual final matrix:\n" << actualFinal_output.toString() << std::endl;

            this->logResult(FINAL_MATRIX, actualFinal_output == expectedFinal);
            this->logResult(SUM_TEST_1, actualSum12 == expectedSum12);
            this->logResult(SUM_TEST_2, actualSum34 == expectedSum34);
            this->logResult(MATCHING_OUTPUTS, actualFinal_cellByCell == actualFinal_output);

            this->logResult(THREAD_COUNT, std::set<std::string>( threadIds.begin(), threadIds.end() ).size() == 2 * N + M);

            std::cout << "----------------------------------------" << std::endl;

            fclose(outputFile);
        } else {
            std::cerr << "Fork failed." << std::endl;
            exit(1);
        }
    }

    std::cout << "\nThe test process is finished." << std::endl;
    this->logResult(SUM_THREAD_SYNCHRONISATION, this->sumSyncDetected);
    this->logResult(MUL_THREAD_SYNCHRONISATION, this->mulSyncDetected);
}

bool TesterModule::parseThreadString(const char* str, int &t, char* id, int &matrix_id, int &x, int &y,
                                     int &value) {
    // Create a string stream to parse the input string
    std::regex pattern(R"(t:\s*(\d+)\s*\|\s*\(([^)]+)\)\s*Matrix(\d+)\s*\((\d+),(\d+)\):([0-9]+))");
    std::string input(str);
    std::smatch matches;

    if (std::regex_search(input, matches, pattern)) {
        t = std::stoi(matches[1]);
        strcpy(id, matches[2].str().c_str());
        matrix_id = std::stoi(matches[3]);
        x = std::stoi(matches[4]);
        y = std::stoi(matches[5]);
        value = std::stoi(matches[6]);
        return true;
    } else {
        return false;
    }
}
