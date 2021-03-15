#ifndef SCANNER_H
#define SCANNER_H

#include "ash.h"

// Prints a graphical representation of the command.
void prncmd(struct command *cmd);

// Scans a buffer and constructs executable command.
void scan(struct command *cmd, char *input);

#endif
