#include "list_dir.h"

// Dirent: directory entry, file or directory

#define DNAME_LENGTH 256

// Context for recursive call of directory walker
typedef struct ctxt {
    // Context from last call
    struct ctxt *last;
    // Current directory (must be a directory), complete path from current
    // directory
    char *dir_name;
} context;

// Create context from another, use NULL for the first context of the list
// May return NULL in case of allocation error
context *create_context(context *last, char *path) {
    context *ctxt = (context *)malloc(sizeof(context));
    if (ctxt == NULL) {
        return NULL;
    }
    char *dir_name = (char *)calloc(DNAME_LENGTH, sizeof(char));
    if (last != NULL)
        strncpy(dir_name, last->dir_name, DNAME_LENGTH);
    strncat(dir_name, "/", DNAME_LENGTH);
    strncat(dir_name, path, DNAME_LENGTH);
    ctxt->dir_name = dir_name;
    ctxt->last = last;
    return ctxt;
}

// Create a new context linked to another (last), from a dirent structure
// dirent d must be a file, the function returns NULL otherwise
context *create_context_from_dirent(context *last, struct dirent *d) {
    if (d->d_type != DT_DIR) {
        return NULL;
    }
    return create_context(last, d->d_name);
}

// TODO Free context
void free_context(context *ctxt) {
    free(ctxt->dir_name);
    free(ctxt);
}

// To filter a path. Returns 1 if the dirent should be ignored
typedef int (*filter)(context *, char[DNAME_LENGTH]);
typedef void (*printer)(context *, char[DNAME_LENGTH]);

// Walk through directories and files, recursively
// Context must point to a folder
// Filter is a NULL terminated array of functions telling whether the file or
// directory should be ignored.
int dir_walker(context *ctxt, filter filters[], printer printer) {
    // Apply filters to a file, to know whether the file is to be explored
    int apply_filter(struct dirent * file) {
        // Whether the file should be ignored
        int ignore = 0;
        // Indice in filter array
        int filter_ind = 0;
        ignore = 0;
        while (!ignore && filters[filter_ind] != NULL) {
            ignore = filters[filter_ind](ctxt, file->d_name);
            filter_ind++;
        }
        return ignore;
    }
    struct dirent *file;
    // Result of filter functions
    DIR *dir = opendir(ctxt->dir_name);
    if (dir == NULL) {
        printf("dir is NULL, %i\n", errno);
        return errno;
    }
    // TODO Treat errno after while loop
    while ((file = readdir(dir)) != NULL) {
        int ignore = apply_filter(file);
        if (!ignore) {
            if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
                // Don’t iterate over current directory
            } else if (file->d_type == DT_REG) { // Regular file
                printer(ctxt, file->d_name);
            } else if (file->d_type == DT_DIR) {
                context *next_ctxt = create_context_from_dirent(ctxt, file);
                dir_walker(next_ctxt, filters, printer);
                free_context(next_ctxt);
            }
            // XXX Ignoring other file types…
        }
    }
    closedir(dir);
    return 0;
}

// Path: name of the directory to explore from
int walk_from(char path[], filter filters[], printer printer) {
    context *ctxt = create_context(NULL, path);
    strncpy(ctxt->dir_name, path, DNAME_LENGTH);
    ctxt->last = NULL;
    return dir_walker(ctxt, filters, printer);
}

void print_name(context *ctxt, char file[]) {
    printf("%s/%s\n", ctxt->dir_name, file);
}

void list() {
    filter filters[] = {NULL};
    walk_from(".", filters, &print_name);
}
