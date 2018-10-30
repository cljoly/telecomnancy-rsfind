#include "image_search.h"

// TODO Place this in a global initialisation function?
// XXX Make this behave correctly with threads…
static magic_t cookie = NULL;

void image_init() {
    cookie = magic_open(MAGIC_MIME );
    return;
}

void image_free() {
  magic_close(cookie);
  return;
}

// A filter, as defined in list_dir.h
// The magic cookie (see libmagic(3)) must have been initiliased
int image_filter(context *ctxt, char *path) {
    const char *file_type = magic_file(cookie, "./simple.png");
    // TODO How this is for images
    printf("FILE TYPE: %s (%s %s)\n", file_type, ctxt->dir_name, path);
    return 0;
}
