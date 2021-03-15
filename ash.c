#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#include "rl/rl.h"
#include "ash.h"
#include "scanner.h"

void exec(struct command *cmd)
{
    if (!strcmp("cd", cmd->prog)) {
        if(-1 == chdir(cmd->args[1])) {
            perror("Error (chdir)");
        }
        return;
    }

    if (!strcmp("exit", cmd->prog)) {
        exit(0);
    }

    // External comand; fork and exec the command
    // in child process.
    pid_t pid = fork();

    if (-1 == pid) {
        perror("Error (fork)");
        exit(1);
    }

    if (!pid) { /* Child */
        // Input redirection
        if (cmd->in) {
            int fd = open(cmd->in, O_RDONLY);

            if (-1 == fd) {
                perror("Error (open)");
                exit(1);
            }

            if (dup2(fd, STDIN_FILENO)) {
                perror("Error (dup2)");
                exit(1);
            }
        }

        // Output redirection
        if (cmd->out) {
            // 0666 = read/write for user (normal)
            int fd = open(cmd->out, O_CREAT|O_WRONLY, 0666);

            if (-1 == fd) {
                perror("Error (open)");
                exit(1);
            }

            if (dup2(fd, STDOUT_FILENO)) {
                perror("Error (dup2)");
                exit(1);
            }
        }

        // Execute the command.
        execvp(cmd->prog, cmd->args); /* Only returns error*/
        perror("Error (exec)");
        exit(1);
    } else { /* Parent */
        waitpid(pid, NULL, 0);
    }
}

void repl()
{
    char *input;
    struct command cmd;

    // Set the path to something reasonable, in the event that
    // the shell is not launched inside another shell.
    setenv("PATH", "/usr/local/bin:/usr/bin:/bin", 1);

    while ((input = readln("$ ", NULL))) {
        scan(&cmd, input);
        exec(&cmd);
    }

    printf("\n(exit)\n");
}

int main(int nargs, char **args)
{
    if (nargs > 1) { /* Script */
        char *filename = args[1];
        printf("TODO: Run script %s\n", filename);
    } else { /* Interactive */
        // Trap SIGINT.
        if (SIG_ERR == signal(SIGINT, SIG_IGN)) {
            perror("Error (signal)");
            exit(1);
        }

        // Run the repl.
        repl();
    }

    return 0;
}
