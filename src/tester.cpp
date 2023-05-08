//
// Created by Baran Yancı on 7.05.2023.
//

#include "tools/matrix.h"

#include "lib/arguments.h"

#include <iostream>

int main(int argc, char** argv) {
    Arguments arguments = parseArguments(argc, argv);
    std::vector<MatrixGroup>* matrixGroups = generateRandomMatrixGroups(arguments.matrixGroupCount, arguments.minDimension, arguments.maxDimension);

    std::cout << "Matrices generated." << std::endl;

    // TODO: 

    delete [] matrixGroups;

    return 0;
}
