#include "list_dir.h"
/* #include "thread.h" */
int *run_n_thread(int nb_thread);
void wait_threads(int *ids, int nb_thread);



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

filter_with_extra *wrap_filter(filter f, char *extra_argument) {
  filter_with_extra  *fwa = (filter_with_extra*)malloc(sizeof(filter_with_extra ));
  fwa->f = f;
  fwa->extra = extra_argument;
  return fwa;
}

printer_with_extra *wrap_printer(printer f, char *extra_argument) {
  printer_with_extra  *fwa = (printer_with_extra*)malloc(sizeof(printer_with_extra ));
  fwa->f = f;
  fwa->extra = extra_argument;
  return fwa;
}

void warn_extra_not_null(char *extra_argument) {
  if (extra_argument != NULL)
    fprintf(stderr, "Warning: extra argument not NULL: %s\n", extra_argument);
}

// Run each printer in a NULL terminated array
void apply_each_printer(printer_with_extra *wrapped_printers[], context *ctxt, char *path) {
  int i = 0;
  printer p = NULL;
  char *extra_argument = NULL;
  while (wrapped_printers[i] != NULL) {
    p = wrapped_printers[i]->f;
    extra_argument = wrapped_printers[i]->extra;
    p(ctxt, path, extra_argument);
    i++;
  }
}

// Apply filters to a file, to know whether the file is to be explored
filter_result apply_each_filter(filter_with_extra *wrapped_filters[], char file_path[], context *ctxt, int is_folder) {
  filter_result action = FILTER_KEEP;
  int filter_ind = 0;
  filter_result filter_result = FILTER_KEEP;
  filter f = NULL;
  char *extra_argument = NULL;
  while (action != FILTER_IGNORE && wrapped_filters[filter_ind] != NULL) {
    f = wrapped_filters[filter_ind]->f;
    extra_argument = wrapped_filters[filter_ind]->extra;
    filter_result = f(ctxt, file_path, is_folder, extra_argument);
    if (filter_result < action) {
      // XXX
      action = filter_result;
    }
    filter_ind++;
  }
  return action;
}


// Walker through directories and files, recursively
// Context must point to a folder
// Filter is a NULL terminated array of functions telling whether the file or
// directory should be ignored.
int dir_walker(context *ctxt, filter_with_extra *wrapped_filters[], printer_with_extra *wrapped_printers[]) {
    struct dirent *file;
    // Return value
    int ret = 0;
    // Result of filter functions
    DIR *dir = opendir(ctxt->dir_name);
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
          filter_action = apply_each_filter(wrapped_filters, file->d_name, ctxt, is_folder);
        }
        // Print current filename or not
        switch (filter_action) {
        case FILTER_IGNORE:
            break;
        case FILTER_CONTINUE:
            break;
        case FILTER_KEEP:
            apply_each_printer(wrapped_printers, ctxt, file->d_name);
            break;
        }
        // Iterate over next folder
        if (filter_action != FILTER_IGNORE && is_folder) {
            context *next_ctxt = create_context_from_dirent(ctxt, file);
            ret = ret || dir_walker(next_ctxt, wrapped_filters, wrapped_printers);
            free_context(next_ctxt);
        }
        // XXX Ignoring other file types…
    }
    closedir(dir);
    return ret;
}

// Path: name of the directory to explore from

int walk_from(char path[], filter_with_extra *wrapped_filters[], printer_with_extra *printers[], int nb_thread) {
    /* int *ids = run_n_thread(nb_thread); */
    fprintf(stderr, "nb_thread %i non utilisé", nb_thread);
    // First printing on path, like printf
    int ret = 0;
    DIR *dir = opendir(path);
    closedir(dir);
    if (dir == NULL) {
      fprintf(stderr, "first dir is NULL, %i\n", errno);
      ret = 1;
      return ret;
    }

    context *ctxt = create_context(NULL, path);
    // Consider the path is always a folder…
    filter_result fr = apply_each_filter(wrapped_filters, path, ctxt, 1);
    if (fr == FILTER_KEEP) {
      apply_each_printer(printers, NULL, path);
    }
    if (fr != FILTER_IGNORE) {
      ret = dir_walker(ctxt, wrapped_filters, printers);
    }

    /* wait_threads(ids, nb_thread); */

    return ret;
}
