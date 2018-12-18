#ifndef TEXTSEARCH_H_INCLUDED
#define TEXTSEARCH_H_INCLUDED

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "list_dir.h"

filter_result text_filter(context *ctxt, char *path, int is_folder, char *extra_argument);

#endif // TEXTSEARCH_H_INCLUDED
