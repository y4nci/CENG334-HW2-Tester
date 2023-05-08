//
// Created by Baran Yancı on 8.05.2023.
//

#ifndef TESTER_HW2_ARGUMENTS_H
#define TESTER_HW2_ARGUMENTS_H

#include <string>
#include <iostream>

#define MATRIX_COUNT 10
#define MIN_DIMENSION 1
#define MAX_DIMENSION 10

struct Arguments {
    int matrixGroupCount;
    int minDimension;
    int maxDimension;
};

Arguments parseArguments(int argc, char** argv);

#endif //TESTER_HW2_ARGUMENTS_H
