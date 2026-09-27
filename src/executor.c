#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "executor.h"
#include "builtin.h"

void execute_pipeline(pipeline_t *pipeline) {
    if (pipeline->command_count == 0) return;

    command_t *cmd = &pipeline->commands[0];
    if (cmd->argc == 0) return;

    if (is_builtin(cmd->argv[0])) {
        execute_builtin(cmd);
    } else {
        pid_t pid = fork();
        if (pid == 0) { 
            // Child process executes the external command
            if (execvp(cmd->argv[0], cmd->argv) == -1) {
                perror("execvp");
                exit(1);
            }
        } else if (pid > 0) { 
            // Parent process waits for the child to finish
            int status;
            waitpid(pid, &status, 0);
        } else {
            perror("fork");
        }
    }
}
