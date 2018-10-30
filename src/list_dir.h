#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LIST_DIR_H
#define LIST_DIR_H

// Length for a file name in various part of dirent
#define DNAME_LENGTH 256

// Context for recursive call of directory walker
// You may sometimes need it in your filter/printer but nowhere else
typedef struct ctxt {
    // Context from last call
    struct ctxt *last;
    // Current directory (must be a directory), complete path from current
    // directory
    char *dir_name;
} context;

// To filter a path. Returns 1 if the dirent should be ignored
typedef int (*filter)(context *, char[DNAME_LENGTH]);
typedef void (*printer)(context *, char[DNAME_LENGTH]);

// Walk through directories and files, recursively
// Path: name of the directory to explore from
// Filter and printer are the function to be used to filter files and to print
// results
int walk_from(char path[], filter filters[], printer printer);

#endif
