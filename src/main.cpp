//
// Created by Baran Yancı on 7.05.2023.
//

#include "lib/arguments.h"
#include "tools/TesterModule.h"

#include <iostream>
#include <sys/fcntl.h>

char executablePath[] = "./hw2";

int main(int argc, char** argv) {
    Arguments arguments = parseArguments(argc, argv);
    TesterModule testerModule(arguments);

    testerModule.confirmExecutable(executablePath);

    std::cout << "Matrices generated." << std::endl;

    testerModule.run();

    return 0;
}
