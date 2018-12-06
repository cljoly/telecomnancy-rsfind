#include "name_search.h"

filter_result name_filter(context *ctxt, char *path, int is_folder, char *name) {
  fprintf(stderr, "ctxt not used: %s\n", ctxt->dir_name);
  if (strncmp(path, name, DNAME_LENGTH) == 0) { // Filename equal name argument
    return FILTER_KEEP;
  } else { if (is_folder) {
    return FILTER_CONTINUE;
  } else {
    return FILTER_IGNORE;
  }
  }
}
