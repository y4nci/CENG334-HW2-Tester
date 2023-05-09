//
// Created by Baran Yancı on 9.05.2023.
//

#ifndef TESTER_HW2_TESTERMODULE_H
#define TESTER_HW2_TESTERMODULE_H

#include "matrix.h"
#include "lib/arguments.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

enum class TestResult {
    FAILURE,
    SUCCESS
};

enum class TestType {
    MATRIX,
    THREAD_COUNT,
    THREAD_ORDERING
};

union TestResultData {
    /**
     * @param first: expected result
     * @param second: actual result
     */
    std::pair<Matrix*, Matrix*> matrices;

    /**
     * @param first: expected count
     * @param second: actual count
     */
    std::pair<int, int> thread_counts;

    /**
     * @param first: expected number of first-multiplier-then-adder instances
     * @param second: actual number of first-multiplier-then-adder instances
     */
    std::pair<int, int> threadOrdering;
};

class TesterModule {
private:
    Arguments arguments;
    std::vector<MatrixGroup>* matrixGroups;
public:
    TesterModule();
    TesterModule(Arguments arguments);
    ~TesterModule();

    void initialiseEnvironment();
    void confirmExecutable(const char* executablePath);
    void run();
};

#endif //TESTER_HW2_TESTERMODULE_H
