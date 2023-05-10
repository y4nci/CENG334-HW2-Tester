//
// Created by Baran Yancı on 9.05.2023.
//

#ifndef TESTER_HW2_TESTERMODULE_H
#define TESTER_HW2_TESTERMODULE_H

#include "matrix.h"
#include "../lib/arguments.h"
#include "../lib/Fork.h"

#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <climits>
#include <regex>
#include <set>

#define SUM_TEST_1 "SUM OF MATRICES A&B"
#define SUM_TEST_2 "SUM OF MATRICES C&D"
#define MATCHING_OUTPUTS "MATCHING OUTPUT MATRICES"
#define FINAL_MATRIX "FINAL MATRIX"
#define M0_THREAD_COUNT "THREAD COUNT FOR ADDING A&B"
#define M1_THREAD_COUNT "THREAD COUNT FOR ADDING C&D"
#define M2_THREAD_COUNT "THREAD COUNT FOR MULTIPLYING J&L"
#define SUM_THREAD_SYNCHRONISATION "THREAD SYNCHRONISATION BETWEEN SUM THREADS"
#define MUL_THREAD_SYNCHRONISATION "THREAD SYNCHRONISATION BETWEEN MUL THREADS AND SUM THREADS"

class TesterModule {
private:
    bool allTestsPassed;
    bool sumSyncDetected;
    bool mulSyncDetected;
    Arguments arguments;
    std::vector<MatrixGroup>* matrixGroups;
public:
    TesterModule();
    TesterModule(Arguments arguments);
    ~TesterModule();

    void initialiseEnvironment();
    void confirmExecutable(const char* executablePath);
    void run(int testNumber);
    void evaluate(const char* title, bool passed);
    bool parseThreadString(const char* str, int& t, char* id, int& matrix_id, int& x, int& y, int& value);
    void endTest();
};

#endif //TESTER_HW2_TESTERMODULE_H
