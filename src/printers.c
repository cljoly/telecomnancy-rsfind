#include "printers.h"

void basic_printer(context *ctxt, char file[]) {
    if (ctxt == NULL) {
        printf("%s\n", file);
    } else {
        printf("%s/%s\n", ctxt->dir_name, file);
    }
}

void complete_printer(context *ctxt, char file[]) {
    char path[256];
    complete_path(ctxt, file, path);
    struct stat sb;
    stat(path, &sb);

        //file type https://linux.die.net/man/2/stat
    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  printf("b ");        break;
        case S_IFCHR:  printf("c ");        break;
        case S_IFDIR:  printf("d ");        break;
        case S_IFIFO:  printf("p ");        break;
        case S_IFLNK:  printf("l ");        break;
        case S_IFREG:  printf("- ");        break;
        case S_IFSOCK: printf("s ");        break;
        default:       printf("- ");        break;
    }

        //file permissions https://linux.die.net/man/2/chmod
    (sb.st_mode & S_IRUSR) ? printf("r") : printf("-");
    (sb.st_mode & S_IWUSR) ? printf("w") : printf("-");
    (sb.st_mode & S_IXUSR) ? printf("x") : printf("-");
    (sb.st_mode & S_IRGRP) ? printf("r") : printf("-");
    (sb.st_mode & S_IWGRP) ? printf("w") : printf("-");
    (sb.st_mode & S_IXGRP) ? printf("x") : printf("-");
    (sb.st_mode & S_IROTH) ? printf("r") : printf("-");
    (sb.st_mode & S_IWOTH) ? printf("w") : printf("-");
    (sb.st_mode & S_IXOTH) ? printf("x") : printf("-");

        //number of hard links
    printf(" %lu ", sb.st_nlink);

        //owner http://linux.die.net/man/3/getpwuid
    printf("%s ", (getpwuid(sb.st_uid))->pw_name);

        //group http://linux.die.net/man/3/getgrgid
    printf("\t%s ", (getgrgid(sb.st_gid))->gr_name);

        //size
    printf("\t%lld ", (long long) sb.st_size);

        //date&time of last modification
    char date[24];
    long int time = sb.st_ctime;
    strftime(date, 16, "%b.  %d %H:%M", localtime(&time));
    date[0] = date[0]+32;
    printf("\t%s ", date);
    
    
		
		 //file name
		printf("\t%s\n", file);


		
		
}
