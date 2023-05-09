//
// Created by Baran Yancı on 7.05.2023.
//

#include "tester_output.h"

void log(TestType testType, TestResult testResult, TestResultData testResultData, int testNumber) {
    std::string testTypeString;
    switch (testType) {
        case TestType::MATRIX:
            if (testResult == TestResult::SUCCESS) {
                std::cout << "Matrix Multiplication Test-" << testNumber << " passed." << std::endl;
            } else {
                std::cout << "Matrix Multiplication Test-" << testNumber << " failed." << std::endl;
                std::cout << "Expected:\n" << testResultData.matrices.first << std::endl;
                std::cout << "Actual:\n" << testResultData.matrices.second << std::endl;
            }
            break;
        case TestType::THREAD_COUNT:
            if (testResult == TestResult::SUCCESS) {
                std::cout << "Thread Count Test-" << testNumber << " passed." << std::endl;
            } else {
                std::cout << "Thread Count Test-" << testNumber << " failed." << std::endl;
                std::cout << "Expected Thread Count:\n" << testResultData.thread_counts.first << std::endl;
                std::cout << "Actual Thread Count:\n" << testResultData.thread_counts.second << std::endl;
            }
            break;
        case TestType::THREAD_ORDERING:
            if (testResult == TestResult::SUCCESS) {
                std::cout << "Thread Ordering Test passed." << std::endl;
            } else {
                std::cout << "Thread Ordering Test failed." << std::endl;
            }
            break;
        default:
            break;
    }
}
