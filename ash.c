#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "rl/rl.h"
#include "ash.h"
#include "scanner.h"

void exec(struct command *cmd)
{
    // Internal commands.
    if (!strcmp("cd", cmd->prog)) {
        chdir(cmd->args[1]);
        return;
    }

    if (!strcmp("exit", cmd->prog)) {
        exit(0);
    }

    // External comand; fork and exec.
    pid_t pid = fork();

    if (!pid) { /* Child */

        // Input redirection
        if (cmd->in) {
            int fd = open(cmd->in, O_RDONLY);
            dup2(fd, STDIN_FILENO);
        }

        // Output redirection
        if (cmd->out) {
            // 0666 = read/write for user (normal)
            int fd = open(cmd->out, O_CREAT | O_WRONLY, 0666);
            dup2(fd, STDOUT_FILENO);
        }

        // Execute the command.
        int status = execv(cmd->prog, cmd->args);
    } else { /* Parent */
        waitpid(pid, NULL, 0);
    }
}

void shell()
{
    char *input;
    struct command cmd;

    while ((input = readln("$ ", NULL))) {
        scan(&cmd, input);
        exec(&cmd);
    }

    printf("\n(exit)\n");
}

int main()
{
    // Enter raw mode.
    raw_mode();
    // Run the shell.
    shell();
    return 0;
}
