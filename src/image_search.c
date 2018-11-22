#include "image_search.h"
#include <string.h>

// TODO Place this in a global initialisation function?
// XXX Make this behave correctly with threads…
static magic_t cookie = NULL;

void image_init() {
  fprintf(stderr, "img init\n");
    cookie = magic_open(MAGIC_MIME_TYPE);
    magic_load(cookie, NULL);
    return;
}

void image_free() {
  if (cookie != NULL) {
    magic_close(cookie);
  }
  return;
}

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

// Checks if a mime type is an image mime type
int is_image_mime(const char mime_type[]) {
  // strncmp retuns 0 if file_type starts with "image/"
  int is_not_matching = strncmp("image/", mime_type, 6);
  return !is_not_matching;
}

// A filter, as defined in list_dir.h
// The magic cookie (see libmagic(3)) must have been initiliased
filter_result image_filter(context *ctxt, char *path) {
  char cpl_path[DNAME_LENGTH];
  complete_path(ctxt, path, cpl_path);
  const char *file_type = magic_file(cookie, cpl_path);
  fprintf(stderr, "FILE TYPE: '%s' (%s %s)\n", file_type, path, ctxt->dir_name);
  int traverse = is_in_set(traverse_mimes, file_type);
  if (traverse) {
    return FILTER_KEEP;
  } else {
    int is_image = is_image_mime(file_type);
    if (is_image) return FILTER_KEEP;
    else return FILTER_IGNORE;
  }
}
