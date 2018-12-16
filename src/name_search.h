#include "list_dir.h"
#include <fnmatch.h>

// Filter file where name is equal to the extra argument
filter_result name_filter(context*, char*, int, char*);

// Filter file where the name matches the extra argument regexp
// Works much like the --name option of find, so directories are displayed if it
// matches
filter_result ename_filter(context*, char*, int, char* regexp);
