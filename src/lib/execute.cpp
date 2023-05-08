//
// Created by Baran Yancı on 8.05.2023.
//

#include <sys/fcntl.h>
#include "execute.h"

void confirmExecutable(const char* executablePath) {
    if (access(executablePath, X_OK) == -1) {
        std::cerr << "Executable is not found, or not executable." << std::endl;
        exit(1);
    }
}

void execute(const char* executablePath, const char* outputPath) {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        dup2(open(outputPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU), STDOUT_FILENO);
        execl(executablePath, executablePath, NULL);
    } else if (pid > 0) {
        // Parent process
        int status;

        waitpid(pid, &status, 0);

        // read from output file, and evaluate the result.

        // TODO:
    } else {
        std::cerr << "Fork failed." << std::endl;
        exit(1);
    }
}
