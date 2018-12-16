#include "pcre_search.h"

#define BUFF_INCREMENT 1024
#define OVECCOUNT 30

// Read fd to fill the pointed buffer with a complete line, reallocating if required
// Update buf_size and returns the number of read characters
int fill_buffer_line(char **buff_ptr, int *buff_size, int fd) {
  char c;
  int buff_pos = 0;
  while(read(fd, &c, 1) > 0 && strncmp(&c,"\n", 1) != 0) {
    buff_pos++;
    if (buff_pos >= *buff_size) {
      *buff_size += BUFF_INCREMENT;
      *buff_ptr = realloc(*buff_ptr, *buff_size);
    }
    (*buff_ptr)[buff_pos] = c;
  }
  return buff_pos;
}

int pcre_search(context *ctxt, char *path, char *pattern) {
    char filepath[DNAME_LENGTH];
    complete_path(ctxt, path, filepath);
    int fd = open(filepath, O_RDONLY, 0);

    int buff_size = BUFF_INCREMENT;
    char *buff_ptr = (char *)malloc(BUFF_INCREMENT * sizeof(char));

    int ovector[OVECCOUNT];
    const char *error;
    int erroroffset = 0;
    // TODO Compile regexp only once
    pcre *re = pcre_compile(pattern, 0, &error, &erroroffset, NULL);
    if (re == NULL)
      fprintf(stderr, "PCRE failed %d: %s\n", erroroffset, error);

    int path_len = strlen(path);

    while (fill_buffer_line(&buff_ptr, &buff_size, fd) > 0) {
      int rc = pcre_exec(re, NULL, path, path_len, 0, 0, ovector, OVECCOUNT);
      if (rc < 0) {
        switch (rc) {
        case PCRE_ERROR_NOMATCH: break;
        default:
          fprintf(stderr, "Error with PCRE %d\n", rc);
          break;
        }
      } else {
        // Matched
        return FILTER_KEEP;
      }
    }

    pcre_free(re);
    close(fd);
    free(buff_ptr);
    return FILTER_IGNORE;
}

filter_result pcre_filter(context *ctxt, char *path, int is_folder, char *extra_argument) {
  if (is_folder) {
      return FILTER_CONTINUE;
  }
  return pcre_search(ctxt, path, extra_argument);
}
