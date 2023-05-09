//
// Created by Baran Yancı on 9.05.2023.
//

#include "TesterModule.h"

TesterModule::TesterModule() {
}

TesterModule::TesterModule(Arguments arguments) {
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
    this->matrixGroups = generateRandomMatrixGroups(arguments.matrixGroupCount, arguments.minDimension, arguments.maxDimension);
}

void TesterModule::run() {
    const char executablePath[] = "./hw2";

    for (int i = 0; i < arguments.matrixGroupCount; i++) {
        char outputPath[16];
        pid_t pid = fork();

        snprintf(outputPath, 16, "./output%d.txt", i);

        if (pid == 0) {
            // Child process
            dup2(open(outputPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU), STDOUT_FILENO);
            execl(executablePath, executablePath, NULL);
        } else if (pid > 0) {
            // Parent process
            int status, threadLines = 0;
            unsigned finalRow = matrixGroups->at(i).values[0].getMatrixValues().size()
                    , finalColumn = matrixGroups->at(i).values[3].getMatrixValues()[0].size();
            FILE *outputFile;
            Matrix expectedSum12 = matrixGroups->at(i).values[0] + matrixGroups->at(i).values[1]
                    , expectedSum34 = matrixGroups->at(i).values[2] + matrixGroups->at(i).values[3]
                    , expectedFinal = expectedSum12 * expectedSum34
                    , actualSum12, actualSum34, actualFinal;

            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                std::cout << "Child process exited with status " << WEXITSTATUS(status) << std::endl;
            } else {
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
                        actualFinal[x][y] = value;
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
                            actualFinal[row][column++] = atoi(token);
                        }
                    }
                }
            }

            // TODO: output
        } else {
            std::cerr << "Fork failed." << std::endl;
            exit(1);
        }
    }
}
