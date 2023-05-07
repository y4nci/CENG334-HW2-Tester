//
// Created by Baran Yancı on 7.05.2023.
//

#include "thread.h"

#include <cstdio>
#include <cstdlib>

/**
 * gets the thread count of the given process
 * @return
 */
int getThreadCount(pid_t pid) {
    char command[50];
    sprintf(command, "ps -T -p %d | wc -l", pid);
    FILE *fp = popen(command, "r");
    char result[50];
    fgets(result, 50, fp);
    pclose(fp);
    return atoi(result) - 2;
}
