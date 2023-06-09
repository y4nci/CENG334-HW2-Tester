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

    if (this->allTestsPassed) {
        std::cout << "All tests passed." << std::endl;
        system("rm -rf logs/output*.txt");
    } else {
        std::cout << "Some tests failed." << std::endl;
        std::cout << "You can see the outputs your code produced by looking at 'logs/output{test case #}.txt'." << std::endl;
    }
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
    this->allTestsPassed = true;
    this->matrixGroups = generateRandomMatrixGroups(arguments.matrixGroupCount, arguments.minDimension, arguments.maxDimension);

    std::cout << "Environment initialised." << std::endl;
}

void TesterModule::evaluate(const char* title, bool passed) {
    std::cout << title << ": " << (passed ? "PASSED" : "FAILED") << std::endl;

    if (!passed) {
        this->allTestsPassed = false;
    }
}

void TesterModule::run(int testNumber) {
    const char executablePath[] = "./hw2";

    std::cout << "Tester started." << std::endl;

    char outputPath[24];
    pid_t pid;
    int fd[2];

    snprintf(outputPath, 24, "logs/output%d.txt", testNumber+1);

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
        unsigned finalRow = matrixGroups->at(testNumber).values[0].getMatrixValues().size()
                , finalColumn = matrixGroups->at(testNumber).values[3].getMatrixValues()[0].size()
                , N = finalRow, K = finalColumn, M = matrixGroups->at(testNumber).values[0].getMatrixValues()[0].size();
        FILE *outputFile;
        Matrix expectedSum12 = matrixGroups->at(testNumber).values[0] + matrixGroups->at(testNumber).values[1]
                , expectedSum34 = matrixGroups->at(testNumber).values[2] + matrixGroups->at(testNumber).values[3]
                , expectedFinal = expectedSum12 * expectedSum34
                , actualSum12(N, M, 0), actualSum34(M, K, 0)

                /* actualFinal_cellByCell is a matrix containing the values it read from the outputs of the threads.
                    * whereas actualFinal_output is a matrix containing the values it read from the end of the output file.
                    */
                , actualFinal_cellByCell(N, K, 0), actualFinal_output(N, K, 0);
        std::vector<std::string> threadIdsM0, threadIdsM1, threadIdsM2;

        close(fd[0]);

        for (int j = 0; j < 4; j++) {
            std::cout << "Input matrix " << j+1 << " for case #" << testNumber+1 << "\n";
            std::cout << matrixGroups->at(testNumber).values[j].toString() << std::endl;
            write(fd[1], matrixGroups->at(testNumber).values[j].toString().c_str(), matrixGroups->at(testNumber).values[j].toString().length());
        }

        close(fd[1]);
        waitpid(pid, &status, 0);

        if (!WIFEXITED(status)) {
            std::cerr << "Child process exited abnormally." << std::endl;
            this->evaluate(SUM_TEST_1, false);
            this->evaluate(SUM_TEST_2, false);
            this->evaluate(MATCHING_OUTPUTS, false);
            this->evaluate(FINAL_MATRIX, false);
            this->evaluate(M0_THREAD_COUNT, false);
            this->evaluate(M1_THREAD_COUNT, false);
            this->evaluate(M2_THREAD_COUNT, false);
            std::cout << "----------------------------------------" << std::endl;
            return;
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
            char line[256];

            fgets(line, 256, outputFile);

            if (line[0] == 't') {
                char id[64]; // TODO: use this id to check whether a row is actually computed by the thread that is supposed to compute it.
                int time, x, y, value, matrixId;

                if (!parseThreadString(line, time, id, matrixId, x, y, value)) {
                    std::cerr << "Thread string could not be parsed." << std::endl;
                    exit(1);
                }

                if (matrixId == 0) {
                    if (x > N || y > M) {
                        std::cerr << "Matrix0 is out of bounds." << std::endl;
                    } else {
                        actualSum12[x-1][y-1] = value;
                    }
                    threadIdsM0.push_back(id);
                } else if (matrixId == 1) {
                    if (x > M || y > K) {
                        std::cerr << "Matrix1 is out of bounds." << std::endl;
                    } else {
                        actualSum34[x-1][y-1] = value;
                    }
                    threadIdsM1.push_back(id);
                } else if (matrixId == 2) {
                    if (x > N || y > K) {
                        std::cerr << "Matrix2 is out of bounds." << std::endl;
                    } else {
                        actualFinal_cellByCell[x-1][y-1] = value;
                    }
                    threadIdsM2.push_back(id);
                }

                if (lastMatrixId == 2 && matrixId != 2) {
                    this->mulSyncDetected = true;
                } else if (lastMatrixId == 1 && matrixId == 0) {
                    this->sumSyncDetected = true;
                } else {
                    lastMatrixId = matrixId;
                }
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

        std::cout << "\nMatrix group " << testNumber+1 << " is tested.\n\n" << std::endl;

        std::cout << "Expected sum of matrix 1 and 2:\n" << expectedSum12.toString() << std::endl;
        std::cout << "Actual sum of matrix 1 and 2:\n" << actualSum12.toString() << std::endl;
        std::cout << "Expected sum of matrix 3 and 4:\n" << expectedSum34.toString() << std::endl;
        std::cout << "Actual sum of matrix 3 and 4:\n" << actualSum34.toString() << std::endl;
        std::cout << "Expected final matrix:\n" << expectedFinal.toString() << std::endl;
        std::cout << "Actual final matrix:\n" << actualFinal_output.toString() << std::endl;

        this->evaluate(FINAL_MATRIX, actualFinal_output == expectedFinal);
        this->evaluate(SUM_TEST_1, actualSum12 == expectedSum12);
        this->evaluate(SUM_TEST_2, actualSum34 == expectedSum34);
        this->evaluate(MATCHING_OUTPUTS, actualFinal_cellByCell == actualFinal_output);
        this->evaluate(M0_THREAD_COUNT, std::set<std::string>( threadIdsM0.begin(), threadIdsM0.end() ).size() == N);
        this->evaluate(M1_THREAD_COUNT, std::set<std::string>( threadIdsM1.begin(), threadIdsM1.end() ).size() == M);
        this->evaluate(M2_THREAD_COUNT, std::set<std::string>( threadIdsM2.begin(), threadIdsM2.end() ).size() == N);

        std::cout << "----------------------------------------" << std::endl;

        fclose(outputFile);
    } else {
        std::cerr << "Fork failed." << std::endl;
        exit(1);
    }
}

bool TesterModule::parseThreadString(const char* str, int &t, char* id, int &matrix_id, int &x, int &y,
                                     int &value) {
    // Create a string stream to parse the input string
    std::regex pattern("t:\\s*(\\d+)\\s*\\|\\s*\\(([^)]+)\\)\\s*Matrix(\\d+)\\s*\\((\\d+),(\\d+)\\):([0-9]+)");
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

void TesterModule::endTest() {
    std::cout << "\nThe test process is finished." << std::endl;
    this->evaluate(SUM_THREAD_SYNCHRONISATION, this->sumSyncDetected);
    this->evaluate(MUL_THREAD_SYNCHRONISATION, this->mulSyncDetected);

    if (!this->sumSyncDetected || !this->mulSyncDetected) {
        char infoMsg [] = "\tNote that these two units check whether two instances of additions occured concurrently "
                          "and addition and multiplication occured concurrently, respectively. Sometimes "
                          "the results of these test might be `FAILED` but that does not mean your code "
                          "does not work properly, your code just did not execute additions and multiplication "
                          "concurrently for some reason, which is the case for small dimensions usually.";

        std::cout << "\n" << infoMsg << std::endl;
    }
}
