//
// Created by Baran Yancı on 9.05.2023.
//

#include "Fork.h"

pid_t Fork() {
    pid_t pid;
    if ((pid = fork()) < 0) {
        std::cerr << "Fork error" << std::endl;
        exit(1);
    }
    return pid;
}
