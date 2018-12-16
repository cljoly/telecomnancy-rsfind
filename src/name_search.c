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

filter_result ename_filter(context *ctxt, char *path, int is_folder, char *glob) {
  fprintf(stderr, "ctxt, is_folder not used: %s %i\n", ctxt->dir_name, is_folder);
  switch (fnmatch(glob, path, 0)) {
  case 0: { // Filename matches
    fprintf(stderr, "0, %s/%s", ctxt->dir_name, path);
    return FILTER_KEEP;
    break;
  }
  case FNM_NOMATCH: {
    fprintf(stderr, "FNM_NOMATCH, %s/%s", ctxt->dir_name, path);
    if (is_folder)
      return FILTER_CONTINUE;
    return FILTER_IGNORE;
    break;
  }
  default: {
    fprintf(stderr, "Error with fnmatch, %s/%s", ctxt->dir_name, path);
    return FILTER_IGNORE;
    break;
    }
  }
}
