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

void exec(char* commandd);

#endif // EXEC_H_INCLUDED

