/**
 * @file Fork.h
 * @brief This file contains the Fork function, which is a wrapper for the fork system call.
 */

#include "Fork.h"

#include <unistd.h>
#include <iostream>

int Fork() {
    int pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed." << std::endl;
        exit(1);
    }

    return pid;
}
