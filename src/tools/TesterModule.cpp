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
        char outputPath[16];
        pid_t pid = Fork();
        int fd[2];

        snprintf(outputPath, 16, "./logs/output%d.txt", i);

        // DEBUG

        pipe(fd);

        if (pid == 0) {
            // Child process

            close(fd[1]);
            dup2(open(outputPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU), STDOUT_FILENO);
            dup2(fd[0], STDIN_FILENO);
            execl(executablePath, executablePath, NULL);
        } else if (pid > 0) {
            // Parent process

            int status, threadLines = 0, lastMatrixId = INT_MIN;
            unsigned finalRow = matrixGroups->at(i).values[0].getMatrixValues().size()
                    , finalColumn = matrixGroups->at(i).values[3].getMatrixValues()[0].size()
                    , N = finalRow, K = finalColumn, M = matrixGroups->at(i).values[0].getMatrixValues()[0].size();
            FILE *outputFile;
            Matrix expectedSum12 = matrixGroups->at(i).values[0] + matrixGroups->at(i).values[1]
                    , expectedSum34 = matrixGroups->at(i).values[2] + matrixGroups->at(i).values[3]
                    , expectedFinal = expectedSum12 * expectedSum34
                    , actualSum12, actualSum34

                    /* actualFinal_cellByCell is a matrix containing the values it read from the outputs of the threads.
                     * whereas actualFinal_output is a matrix containing the values it read from the end of the output file.
                     */
                    , actualFinal_cellByCell, actualFinal_output;

            close(fd[0]);

            for (int j = 0; j < 4; j++) {
                std::cout << "Input matrix " << j << " for case #\n";
                std::cout << i << matrixGroups->at(i).values[j].toString() << std::endl;
                write(fd[1], matrixGroups->at(i).values[j].toString().c_str(), matrixGroups->at(i).values[j].toString().length());
            }

            waitpid(pid, &status, 0);

            if (!WIFEXITED(status)) {
                std::cerr << "Child process exited abnormally." << std::endl;
                std::cout << "Aborting..." << std::endl;
                exit(1);
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

            while (!feof(outputFile)) {
                char line[64];

                fgets(line, 64, outputFile);

                if (line[0] == 't') {
                    char id[64];
                    int time, x, y, value, matrixId;

                    sscanf(line, "t: %d | (%s) Matrix%d (%d,%d):%d", &time, id, &matrixId, &x, &y, &value);

                    if (matrixId == 0) {
                        actualSum12[x][y] = value;
                    } else if (matrixId == 1) {
                        actualSum34[x][y] = value;
                    } else if (matrixId == 2) {
                        actualFinal_cellByCell[x][y] = value;
                    }

                    if (lastMatrixId == 2 && matrixId != 2) {
                        this->mulSyncDetected = true;
                    } else if (lastMatrixId == 1 && matrixId == 0) {
                        this->sumSyncDetected = true;
                    } else {
                        lastMatrixId = matrixId;
                    }

                    threadLines++;
                } else {
                    // parse final matrix
                    // row by row

                    for (int row = 0; row < finalRow; row++) {
                        char rowString[64];
                        int column = 0;

                        fgets(rowString, 64, outputFile);

                        for (char *token = strtok(rowString, " "); token != NULL; token = strtok(NULL, " ")) {
                            actualFinal_output[row][column++] = atoi(token);
                        }
                    }
                }
            }

            std::cout << "Matrix group " << i << " is tested." << std::endl;

            this->logResult(SUM_TEST_1, actualSum12 == expectedSum12);
            this->logResult(SUM_TEST_2, actualSum34 == expectedSum34);
            this->logResult(MATCHING_OUTPUTS, actualFinal_cellByCell == actualFinal_output);
            this->logResult(FINAL_MATRIX, actualFinal_output == expectedFinal);

            this->logResult(THREAD_COUNT, threadLines == 2 * N + M);

            fclose(outputFile);
            system("rm -f logs/output*.txt");
        } else {
            std::cerr << "Fork failed." << std::endl;
            exit(1);
        }
    }

    this->logResult(SUM_THREAD_SYNCHRONISATION, this->sumSyncDetected);
    this->logResult(MUL_THREAD_SYNCHRONISATION, this->mulSyncDetected);
}
