#include "image_search.h"
#include <string.h>

// TODO Place this in a global initialisation function?
// XXX Make this behave correctly with threads…
static magic_t cookie = NULL;

void image_init() {
    cookie = magic_open(MAGIC_MIME_TYPE);
    magic_load(cookie, NULL);
    return;
}

void image_free() {
  magic_close(cookie);
  return;
}

// Known types for image
char *img_mimes[] = {
  "image/svg",
  "image/png",
  "image/jpeg",
  "image/jpeg",
  "image/png",
  "image/tiff",
  "image/jpeg",
  "application/pdf",
  "image/svg+xml",
  "image/x-ms-bmp",
  NULL
};

// Types to be traversed
// TODO Take other types into account
char *traverse_mimes[] = {
  "inode/directory",
  NULL
};

// Is the string in the set (NULL-terminated array)?
int is_in_set(char *set[], const char *str) {
  int is_not_in_set = 0;
  int set_indice = 0;
  while(!is_not_in_set && set[set_indice] != NULL) {
    is_not_in_set = strcmp(str, set[set_indice]);
    set_indice++;
  }
  if (is_not_in_set)
    return 0;
  else
    return 1;
}

// A filter, as defined in list_dir.h
// The magic cookie (see libmagic(3)) must have been initiliased
int image_filter(context *ctxt, char *path) {
  char cpl_path[DNAME_LENGTH];
  complete_path(ctxt, path, cpl_path);
  const char *file_type = magic_file(cookie, cpl_path);
  fprintf(stderr, "FILE TYPE: '%s' (%s %s)\n", file_type, ctxt->dir_name, path);
  int traverse = is_in_set(traverse_mimes, file_type);
  fprintf(stderr, "traverse %i\n", traverse);
  if (traverse) {
    return FILTER_KEEP;
  } else {
    int is_image = is_in_set(img_mimes, file_type);
    fprintf(stderr, "is_image %i\n", is_image);
    return !is_image;
  }
}
