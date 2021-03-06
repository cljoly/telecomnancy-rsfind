#include "printers.h"

void basic_printer(context *ctxt, char file[], char *extra_argument) {
  warn_extra_not_null(extra_argument);
  char *path = (char *)malloc(DNAME_LENGTH*sizeof(char));
  complete_path(ctxt, file, path);
  printf("%s\n", path);
}

void complete_printer(context *ctxt, char file[], char *extra_argument) {
    warn_extra_not_null(extra_argument);
    char path[256];
    complete_path(ctxt, file, path);
    struct stat sb;
    stat(path, &sb);

    // file type https://linux.die.net/man/2/stat
    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
        printf("b");
        break;
    case S_IFCHR:
        printf("c");
        break;
    case S_IFDIR:
        printf("d");
        break;
    case S_IFIFO:
        printf("p");
        break;
    case S_IFLNK:
        printf("l");
        break;
    case S_IFREG:
        printf("-");
        break;
    case S_IFSOCK:
        printf("s");
        break;
    default:
        printf("-");
        break;
    }

    // file permissions https://linux.die.net/man/2/chmod
    (sb.st_mode & S_IRUSR) ? printf("r") : printf("-");
    (sb.st_mode & S_IWUSR) ? printf("w") : printf("-");
    (sb.st_mode & S_IXUSR) ? printf("x") : printf("-");
    (sb.st_mode & S_IRGRP) ? printf("r") : printf("-");
    (sb.st_mode & S_IWGRP) ? printf("w") : printf("-");
    (sb.st_mode & S_IXGRP) ? printf("x") : printf("-");
    (sb.st_mode & S_IROTH) ? printf("r") : printf("-");
    (sb.st_mode & S_IWOTH) ? printf("w") : printf("-");
    (sb.st_mode & S_IXOTH) ? printf("x") : printf("-");

    // number of hard links
    printf(" %lu", sb.st_nlink);

    // owner http://linux.die.net/man/3/getpwuid
    printf(" %s", (getpwuid(sb.st_uid))->pw_name);

    // group http://linux.die.net/man/3/getgrgid
    printf(" %s", (getgrgid(sb.st_gid))->gr_name);

    // size
    printf(" %lld", (long long)sb.st_size);

    // date&time of last modification
    char date[24];
    time_t now = time (NULL);
    struct tm tmFile, tmNow;
    localtime_r(&sb.st_mtime, &tmFile);
    localtime_r (&now, &tmNow);
    
		if (tmFile.tm_year == tmNow.tm_year) {
            strftime (date, 24, "%b %e %H:%M", &tmFile);
        }
    else { 
            strftime (date, 24, "%b %e  %Y", &tmFile);
        }
		printf(" %s", date);


    // file name
    printf(" %s\n", path);
}
