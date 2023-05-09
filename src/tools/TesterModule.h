//
// Created by Baran Yancı on 9.05.2023.
//

#ifndef TESTER_HW2_TESTERMODULE_H
#define TESTER_HW2_TESTERMODULE_H

#include "matrix.h"
#include "../lib/arguments.h"
#include "../lib/Fork.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#define SUM_TEST_1 "SUM OF MATRICES A&B"
#define SUM_TEST_2 "SUM OF MATRICES C&D"
#define MATCHING_OUTPUTS "MATCHING OUTPUT MATRICES"
#define FINAL_MATRIX "FINAL MATRIX"
#define THREAD_COUNT "THREAD COUNT"
#define SUM_THREAD_SYNCHRONISATION "THREAD SYNCHRONISATION BETWEEN SUM THREADS"
#define MUL_THREAD_SYNCHRONISATION "THREAD SYNCHRONISATION BETWEEN MUL THREADS AND SUM THREADS"

class TesterModule {
private:
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
    void run();
    void logResult(const char* title, bool passed);
};

#endif //TESTER_HW2_TESTERMODULE_H
