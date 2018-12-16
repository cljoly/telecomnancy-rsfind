#ifndef EXEC_H_INCLUDED
#define EXEC_H_INCLUDED

#include "list_dir.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARG_SZ 64
#define MAX_PIPE_NB 16

void exec(char* path, char* command);
void exec_printer(context *ctxt, char file[], char *cmd);
void free_args (char* commands[MAX_PIPE_NB][MAX_ARG_SZ], int arg_nb[MAX_PIPE_NB], int pipe_nb);

#endif // EXEC_H_INCLUDED

