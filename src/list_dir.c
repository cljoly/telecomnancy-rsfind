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
    complete_path(last, path, dir_name);
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
    strcpy(result, path);
  } else {
    int dir_len = strlen(ctxt->dir_name);
    const char *last_dir = ctxt->dir_name+dir_len-1;
    strncpy(result, ctxt->dir_name, DNAME_LENGTH);
    if (strncmp(last_dir, "/", 1) != 0) {
      strncat(result, "/", DNAME_LENGTH);
    }
    strncat(result, path, DNAME_LENGTH);
  }
}

// TODO Free context
void free_context(context *ctxt) {
    free(ctxt->dir_name);
    free(ctxt);
}

// Run each printer in a NULL terminated array
void apply_each_printer(printer printers[], context *ctxt, char *path) {
  int i = 0;
  while (printers[i] != NULL) {
    printers[i](ctxt, path);
    i++;
  }
}

// Walker through directories and files, recursively
// Context must point to a folder
// Filter is a NULL terminated array of functions telling whether the file or
// directory should be ignored.
int dir_walker(context *ctxt, filter filters[], printer printers[]) {
    // Apply filters to a file, to know whether the file is to be explored
    filter_result apply_filter(struct dirent * file, int is_folder) {
        filter_result action = FILTER_KEEP;
        int filter_ind = 0;
        filter_result filter_result = FILTER_KEEP;
        while (action != FILTER_IGNORE && filters[filter_ind] != NULL) {
            filter_result = filters[filter_ind](ctxt, file->d_name, is_folder);
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
    // Return value
    int ret = 0;
    if (dir == NULL) {
        fprintf(stderr, "dir is NULL, %i\n", errno);
        ret = 1;
        return ret;
    }
    // TODO Treat errno after while loop
    while (!ret && (file = readdir(dir)) != NULL) {
        int is_folder = file->d_type == DT_DIR;
        // Apply filter
        filter_result filter_action;
        if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
            // Don’t iterate over current directory
            filter_action = FILTER_IGNORE;
        } else {
            filter_action = apply_filter(file, is_folder);
        }
        // Print current filename or not
        switch (filter_action) {
        case FILTER_IGNORE:
            break;
        case FILTER_CONTINUE:
            break;
        case FILTER_KEEP:
            apply_each_printer(printers, ctxt, file->d_name);
            break;
        }
        // Iterate over next folder
        if (filter_action != FILTER_IGNORE && is_folder) {
            context *next_ctxt = create_context_from_dirent(ctxt, file);
            ret = ret || dir_walker(next_ctxt, filters, printers);
            free_context(next_ctxt);
        }
        // XXX Ignoring other file types…
    }
    closedir(dir);
    return ret;
}

// Path: name of the directory to explore from
int walk_from(char path[], filter filters[], printer printers[], int flag_i) {
    // First printing on path, like printf
    // FIXME Don’t use a flag, generalise
    if (!flag_i) {
      apply_each_printer(printers, NULL, path);
    }

    context *ctxt = create_context(NULL, path);
    strncpy(ctxt->dir_name, path, DNAME_LENGTH);
    ctxt->last = NULL;
    return dir_walker(ctxt, filters, printers);
}
