//
// Created by Baran Yancı on 8.05.2023.
//

#include "arguments.h"

Arguments parseArguments(int argc, char** argv) {
    Arguments arguments = {
            .matrixCount = MATRIX_COUNT,
            .minDimension = MIN_DIMENSION,
            .maxDimension = MAX_DIMENSION
    };

    if (argc == 1) {
        return arguments;
    } else if (argc == 2) {
        if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
            std::cout << "Usage: " << argv[0] << " [] [<matrixCount> <minDimension> <maxDimension>] [-h, --help]\n"
                         "Tester for the HW2 of CENG334 course.\n"
                         "\n"
                         "  -h, --help\t\t\t\tDisplay this help and exit.\n"
                         "  <matrixCount>\t\t\t\tNumber of matrices to be generated.\n"
                         "  <minDimension>\t\t\t\tMinimum dimension of matrices to be generated.\n"
                         "  <maxDimension>\t\t\t\tMaximum dimension of matrices to be generated.\n"
                         "\n"
                         "If no arguments are given, the default values are used.\n"
                         "\n"
                         "Report bugs to baran.yanci@metu.edu.tr" << "\n";
        } else {
            std::cout << "Usage: " << argv[0] << " <matrixCount> <minDimension> <maxDimension>" << std::endl;
            exit(1);
        }
    } else if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <matrixCount> <minDimension> <maxDimension>" << std::endl;
        exit(1);
    }

    arguments.matrixCount = std::stoi(argv[1]);
    arguments.minDimension = std::stoi(argv[2]);
    arguments.maxDimension = std::stoi(argv[3]);

    if (arguments.matrixCount <= 0 || arguments.minDimension <= 0 || arguments.maxDimension <= 0) {
        std::cout << "All arguments must be greater than 0." << std::endl;
        exit(1);
    }

    return arguments;
}
