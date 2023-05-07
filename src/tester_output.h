//
// Created by Baran Yancı on 7.05.2023.
//

#ifndef TESTER_HW2_TESTER_OUTPUT_H
#define TESTER_HW2_TESTER_OUTPUT_H

#include <iostream>
#include "tools/matrix.h"

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

void log(TestType testType, TestResult testResult, TestResultData testResultData, int testNumber);

#endif //TESTER_HW2_TESTER_OUTPUT_H
