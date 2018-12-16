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

const int max_arg_size = 64;
const int max_pipe_number = 16;

void exec(char* path, char* command);
void exec_printer(context *ctxt, char file[], char *cmd);
void free_args (char* commands[max_pipe_number][max_arg_size], int arg_nb[max_pipe_number], int pipe_nb);

#endif // EXEC_H_INCLUDED

