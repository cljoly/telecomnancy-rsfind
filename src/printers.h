#include <stdlib.h>
#include "list_dir.h"

// Group several printers here

// A basic printer, displaying only the path
void basic_printer(context *ctxt, char file[]);

// A more complete printer, for -l option
void complete_printer(context *ctxt, char file[]);
