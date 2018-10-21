#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    /////////////options/////////////

    ///////flags
    static int l_flag;
    static int t_flag;
    static int i_flag;
    static int name_flag;
    static int exec_flag;

    ///////values
    char name;
    char exec;

    ///////getting opts
    int c;

    while (1) {
        static struct option long_options[] = {
            /* These options set a flag. */
            {"name", required_argument, &name_flag, 1},
            {"exec", required_argument, &exec_flag, 1},
            {"listing_long", no_argument, 0, 'l'},
            {"chaine", required_argument, 0, 't'},
            {"is_image", no_argument, 0, 'i'},
            {0, 0, 0, 0}};
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "ilt:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 0:
            /* If this option set a flag, do nothing else now. */
            // if (long_options[option_index].flag != 0)
            //  break;
            printf("option %s, index %i", long_options[option_index].name,
                   option_index);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            if (option_index == 0) {
                strcpy(&name, optarg);
                printf("name = %s\n", &name);
            }
            if (option_index == 1) {
                strcpy(&exec, optarg);
                printf("exec = %s\n", &exec);
            }
            break;

        case 'i':
            puts("option -i\n");
            i_flag = 1;
            break;

        case 'l':
            puts("option -l\n");
            l_flag = 1;
            break;

        case 't':
            printf("option -t with value '%s'\n", optarg);
            t_flag = 1;
            break;

        case '?':
            /* getopt_long already printed an error message. */
            break;

        default:
            abort();
        }
    }

    /* Instead of reporting ‘--verbose’
       and ‘--brief’ as they are encountered,
       we report the final status resulting from them. */
    if (name_flag)
        puts("name flag is set");

    if (exec_flag)
        puts("exec flag is set");

    if (l_flag || i_flag || t_flag)
        puts("ok");

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        putchar('\n');
    }

    exit(0);
}
