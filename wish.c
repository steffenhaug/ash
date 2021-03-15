#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "rl/rl.h"
#include "wish.h"
#include "scanner.h"

void exec(struct command *cmd)
{
    pid_t pid = fork();

    if (!pid) { /* Child  */
        int status = execv(cmd->prog, cmd->args);
        exit(status);
    } else {    /* Parent */
        waitpid(pid, NULL, 0);
    }
}

void shell()
{
    char *input;
    struct command cmd;

    while ((input = readln("$ ", NULL))) {
        scan(&cmd, input);
        prncmd(&cmd);
        //exec(&cmd);
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
