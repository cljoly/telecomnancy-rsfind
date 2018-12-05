#include "image_search.h"

// Copied from /usr/include/magic.h
// TODO Use compiler option?
#define MAGIC_MIME_TYPE     0x0000010 /* Return the MIME type */
typedef struct magic_set *magic_t;
// Handle for libmagic
static void *magic_handle = NULL;
// Libmagic functions
static magic_t (*magic_open)(int) = NULL;
static int (*magic_load)(magic_t, const char*) = NULL;
static void (*magic_close)(magic_t) = NULL;
static const char* (*magic_file)(magic_t, const char*) = NULL;

// TODO Place this in a global initialisation function?
// XXX Make this behave correctly with threads…
static magic_t cookie = NULL;

// Dynamic loading of libmagic
void image_init_load_libmagic() {
  magic_handle = dlopen("libmagic.so", RTLD_LAZY);
  if (magic_handle == NULL) {
    fprintf(stderr, "Problem loading libmagic with libdl\n");
  } else {
    fprintf(stderr, "Libmagic loaded\n");
  }
  magic_open = dlsym(magic_handle, "magic_open");
  magic_load = dlsym(magic_handle, "magic_load");
  magic_close = dlsym(magic_handle, "magic_close");
  magic_file = dlsym(magic_handle, "magic_file");
  return;
}

// Unload libmagic
void image_close_unload_libmagic() {
  if (magic_handle != NULL) {
    dlclose(magic_handle);
  }
  // TODO Set loaded function pointers to NULL
  return;
}

// Initialise magic cookie
void image_init_magic_cookie() {
    fprintf(stderr, "img init\n");
    cookie = magic_open(MAGIC_MIME_TYPE);
    magic_load(cookie, NULL);
    return;
}

// Close magic cookie
void image_close_magic_cookie() {
  if (cookie != NULL) {
    magic_close(cookie);
  }
  return;
}

void image_init() {
  image_init_load_libmagic();
  image_init_magic_cookie();
}

void image_close() {
  image_close_magic_cookie();
  image_close_unload_libmagic();
  return;
}

// Types to be traversed
// TODO Take other types into account
char *traverse_mimes[] = {"inode/directory", NULL};

// Is the string in the set (NULL-terminated array)?
int is_in_set(char *set[], const char *str) {
    int is_not_in_set = 0;
    int set_indice = 0;
    while (!is_not_in_set && set[set_indice] != NULL) {
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
filter_result image_filter(context *ctxt, char *path, int is_folder) {
    char cpl_path[DNAME_LENGTH];
    complete_path(ctxt, path, cpl_path);
    const char *file_type = magic_file(cookie, cpl_path);
    fprintf(stderr, "FILE TYPE: '%s' (%s %s)\n", file_type, path,
            ctxt->dir_name);
    // TODO Compare is_folder & traverse, to make sure both are working well
    /* int traverse = is_in_set(traverse_mimes, file_type); */
    if (is_folder) {
        return FILTER_CONTINUE;
    } else {
        int is_image = is_image_mime(file_type);
        if (is_image)
            return FILTER_KEEP;
        else
            return FILTER_IGNORE;
    }
}
