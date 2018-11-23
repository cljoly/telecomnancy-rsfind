#include "list_dir.h"
#include <magic.h>
#include <stdio.h>

void image_init();

void image_close();

filter_result image_filter(context *ctxt, char *path, int is_folder);
