#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LIST_DIR_H
#define LIST_DIR_H

#include "thread.h"

// Length for a file name in various part of dirent
#define DNAME_LENGTH 256

// To code result of filter, to compel ourselves to write it explicitely
// The lowest filter from all result is kept
typedef enum {
  FILTER_IGNORE=18, // Ignore current element
  FILTER_CONTINUE=38, // Continue to explore with current element, but don’t print it
  FILTER_KEEP=180   // Keep current element (print and continue to explore if it is a directory)
} filter_result;

// Context for recursive call of directory walker
// You may sometimes need it in your filter/printer but nowhere else
typedef struct ctxt {
    // Context from last call
    struct ctxt *last;
    // Current directory (must be a directory), complete path from current
    // directory
    char *dir_name;
} context;

// To filter a path. Last arguments tells if current file is a folder or not.
// See filter_result for return type meaning.
// The last char* is an extra argument to pass whatever your filter needs
typedef filter_result (*filter)(context *, char[DNAME_LENGTH], int, char*);
// Function used to print result
// The last char* is an extra argument to pass whatever your filter needs
typedef void (*printer)(context *, char[DNAME_LENGTH], char*);

// To store the extra char* argument in filter & printer
typedef struct filter_with_extra {
  filter f;
  char *extra;
} filter_with_extra;
typedef struct printer_with_extra {
  printer f;
  char *extra;
} printer_with_extra;

// Wrap a filter/printer with its extra argument in a filter/extra_with_extra structure
// This struct is to be free’d with a call to free
filter_with_extra *wrap_filter(filter, char *);
printer_with_extra *wrap_printer(printer, char *);

// Display a warning on stderr if the extra argument passed is not NULL
void warn_extra_not_null(char *extra_argument);

// Walk through directories and files, recursively
// Path: name of the directory to explore from
// Filter and printer are the function to be used to filter files and to print
// results
// Return 1 if there is an error while traversing files, 0 otherwise
int walk_from(char path[], filter_with_extra *wrapped_filters[], printer_with_extra *printer[], int nb_thread);

// Concat path from a context with a filename, in order to create a complete
// path. Stores the result in result.
void complete_path(context *ctxt, char *path, char *result);

#endif
