#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ash.h"
#include "scanner.h"

void prncmd(struct command *cmd)
{
    // Prints a nice display of an executable command.
    for (int i = 0; cmd->args[i] != NULL; i++) {
        printf("%d: [%s]\n", i, cmd->args[i]);
    }

    printf("infile  %s\n", cmd->in  ? cmd->in  : "(stdin)");
    printf("outfile %s\n", cmd->out ? cmd->out : "(stdout)");
}


void scan(struct command *cmd, char *input)
{
    // This parser is "woefully inadequate", and as such
    // extremely fitting for the shell :-) Anything more
    // sophisticated than this warrants use of Lex.

    // Parse command. (Assume 1st word valid)
    char *token = strsep(&input, " \t");
    cmd->prog = token;

    // Parse arguments.
    int i = 0;
    while (token && strcmp(token, "<") && strcmp(token, ">")) {
        cmd->args[i++] = token;
        // Invariant: Keep NULL terminal ahead
        // of least recently parsed argument.
        cmd->args[i] = NULL;
        token = strsep(&input, " \t");
    }

    // Assume to begin with that no io redirection happens.
    cmd->in  = NULL;
    cmd->out = NULL;

    // Parse IO redirection.
    while (token) {
        // Get the file we redirect to/from.
        char *filename = strsep(&input, " \t");

        // Input redirection.
        if (!strcmp("<", token)) {
            cmd->in = filename;
        } 

        // Output redirection.
        if (!strcmp(">", token)) {
            cmd->out = filename;
        }

        token = strsep(&input, " \t");
    }
}
