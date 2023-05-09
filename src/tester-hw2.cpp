//
// Created by Baran Yancı on 7.05.2023.
//

#include "lib/arguments.h"
#include "tools/TesterModule.h"

#include <iostream>
#include <sys/fcntl.h>

char executablePath[] = "./hw2";
char infoMsg[] = "This is not an official tester, and the result produced by this tester is not guaranteed to be correct.\n" \
                 "Please make sure that you have read the assignment description and the report.\n" \
                 "This tester is only for your convenience.\n" \
                 "If you have any questions, please contact me at baran.yanci@metu.edu.tr.\n\n" \
                 "INFORMATION ABOUT TEST UNITS:\n" \
                         "\t- Sum results\n"
                            "\t\t- Sum of matrices A and B\n"
                            "\t\t- Sum of matrices C and D\n"
                         "\t- Matching output matrices\n"
                            "\t\t- Matching the outputs of individual threads with the final output printed by main\n"
                         "\t- Final matrix\n"
                            "\t\t- Matching the final output printed by main with the expected output\n"
                         "\t- Thread count\n"
                            "\t\t- Matching the thread count printed by main with the expected thread count (2N + M)\n"
                         "\t- Thread synchronisation\n"
                            "\t\t- Detecting concurrent additions of A&B and C&D\n"
                            "\t\t- Detecting concurrent multiplication and addition\n";

int main(int argc, char** argv) {
    Arguments arguments = parseArguments(argc, argv);
    TesterModule testerModule(arguments);

    std::cout << infoMsg << std::endl;

    testerModule.confirmExecutable(executablePath);

    testerModule.initialiseEnvironment();

    testerModule.run();

    return 0;
}
