#include <stdlib.h>
#include "list_dir.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

// Group several printers here

// A basic printer, displaying only the path
// Doesn’t use the extra argument
void basic_printer(context *ctxt, char file[], char *extra_argument);

// A more complete printer, for -l option
// Doesn’t use the extra argument
void complete_printer(context *ctxt, char file[], char *extra_argument);
