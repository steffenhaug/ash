#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "rl/rl.h"

struct command {
    char *prog;
    char *args[32];
};

void scan(struct command *cmd, char *input) {
    int i = 0;
    do {
        cmd->args[i] = strsep(&input, " \t");
    } while (cmd->args[i++]);
    cmd->prog = cmd->args[0];
}

void exec(struct command *cmd) {
    pid_t pid = fork();

    if (!pid) { /* Child  */
        int status = execv(cmd->prog, cmd->args);
        exit(status);
    } else {    /* Parent */
        waitpid(pid, NULL, 0);
    }
}

void tab(struct line* input) {}

void shell() {
    char *input;
    struct command cmd;

    while ((input = readln("$ ", tab))) {
        scan(&cmd, input);
        exec(&cmd);
    }
    printf("\n(exit)\n");
}

int main() {
    // Enter raw mode.
    raw_mode();
    // Run the shell.
    shell();
    return 0;
}

