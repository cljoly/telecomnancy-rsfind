#include "list_dir.h"

// Dirent: directory entry, file or directory

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

void complete_path(context *ctxt, char *path, char *result) {
    if (ctxt == NULL) {
        char *path = "./";
        strcpy(result, path);
    } else {
        strncpy(result, ctxt->dir_name, DNAME_LENGTH);
        strncat(result, "/", DNAME_LENGTH);
        strncat(result, path, DNAME_LENGTH);
    }
}

// TODO Free context
void free_context(context *ctxt) {
    free(ctxt->dir_name);
    free(ctxt);
}

// Walker through directories and files, recursively
// Context must point to a folder
// Filter is a NULL terminated array of functions telling whether the file or
// directory should be ignored.
int dir_walker(context *ctxt, filter filters[], printer printer) {
    // Apply filters to a file, to know whether the file is to be explored
    filter_result apply_filter(struct dirent * file) {
        filter_result action = FILTER_KEEP;
        int filter_ind = 0;
        filter_result filter_result = FILTER_KEEP;
        while (action != FILTER_IGNORE && filters[filter_ind] != NULL) {
            filter_result = filters[filter_ind](ctxt, file->d_name);
            if (filter_result < action) {
                // XXX
                action = filter_result;
            }
            filter_ind++;
        }
        return action;
    }
    struct dirent *file;
    // Result of filter functions
    DIR *dir = opendir(ctxt->dir_name);
    if (dir == NULL) {
        fprintf(stderr, "dir is NULL, %i\n", errno);
        return errno;
    }
    // TODO Treat errno after while loop
    while ((file = readdir(dir)) != NULL) {
        // Apply filter
        filter_result filter_action;
        if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
            // Don’t iterate over current directory
            filter_action = FILTER_IGNORE;
        }
        else {
          filter_action = apply_filter(file);
        }
        // Print current filename or not
        switch(filter_action) {
            case FILTER_IGNORE: break;
            case FILTER_CONTINUE: break;
            case FILTER_KEEP:
              printer(ctxt, file->d_name);
              break;

        }
          // Iterate over next folder
          if (filter_action != FILTER_IGNORE && file->d_type == DT_DIR) {
              context *next_ctxt = create_context_from_dirent(ctxt, file);
              dir_walker(next_ctxt, filters, printer);
              free_context(next_ctxt);
          }
          // XXX Ignoring other file types…
    }
    closedir(dir);
    return 0;
}

// Path: name of the directory to explore from
int walk_from(char path[], filter filters[], printer printer) {
    // First printing on path, like printf
    printer(NULL, path);
    context *ctxt = create_context(NULL, path);
    strncpy(ctxt->dir_name, path, DNAME_LENGTH);
    ctxt->last = NULL;
    return dir_walker(ctxt, filters, printer);
}
