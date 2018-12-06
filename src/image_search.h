#include "list_dir.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

// Initialise magic cookie, required to use image_filter
void image_init();

// Free magic cookie, works also if image_init was not called
void image_close();

filter_result image_filter(context *ctxt, char *path, int is_folder, char *extra_argument);
