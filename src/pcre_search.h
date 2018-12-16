#ifndef PCRESEARCH_H_INCLUDED
#define PCRESEARCH_H_INCLUDED

#include "list_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

filter_result pcre_filter(context *ctxt, char *path, int is_folder, char *pattern);

#endif // PCRESEARCH_H_INCLUDED
