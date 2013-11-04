#ifndef USAGE_H
#define USAGE_H

struct mnw_options {
    char *addr;
    int port;
    char *srcd;
    char *tmpd;
};

struct mnw_options *getOptions(int argc, char *argv[]);
char *program_name;

#endif // USAGE_H
