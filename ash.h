#ifndef WISH_H
#define WISH_H
struct command {
    char *prog;
    char *args[32];
    char *in;
    char *out;
};
#endif
