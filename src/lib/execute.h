//
// Created by Baran Yancı on 8.05.2023.
//

#ifndef TESTER_HW2_EXECUTE_H
#define TESTER_HW2_EXECUTE_H

#include <unistd.h>
#include <iostream>

void confirmExecutable(const char* executablePath);
void execute(const char* executablePath, const char* outputPath);

#endif //TESTER_HW2_EXECUTE_H
