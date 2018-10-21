#include <dirent.h>
#include <stdio.h>

// TODO Delete this
/*
int main() {
  char working_dir_name[] = ".";
  DIR *working_dir = opendir(working_dir_name);
  struct dirent *file;
  // TODO Treat errno after while loop
  while ((file=readdir(working_dir)) != NULL) {
    printf("%s/%s\t\t\t%i\t%i\n", working_dir_name, file->d_name, file->d_type, file->d_reclen);
  }
}
*/
