#include "image_search.h"
#include "list_dir.h"
#include "printers.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    /////////////options/////////////

    // These variables will be modified as argument are parsed and
    // then used with walk_from. Default values have been chosen so as to make
    // rsfind behave like find when no arguments are passed.
    char *path = ".";
    // XXX Should be big enough ;-)
    filter_with_extra *wrapped_filters[] = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };
    // Ajout des fonctions de filtre à l’ensemble des filtres, avec création du conteneur de l’argument optionnel
    void add_to_filters(filter f, char *extra_argument) {
      int i = 0;
      while (wrapped_filters[i] != NULL) {
        i++;
      }
      wrapped_filters[i] = wrap_filter(f, extra_argument);
    }
    printer_with_extra *wrapped_printers[] = {
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };
    // Ajout des fonctions d’affachage à l’ensemble des affichages, avec création du conteneur de l’argument optionnel
    void add_to_printers(printer f, char *extra_argument) {
        int i = 0;
        while (wrapped_printers[i] != NULL) {
            i++;
        }
        wrapped_printers[i] = wrap_printer(f, extra_argument);
    }

    ///////values
    char name;
    char exec;

    ///////getting opts
    int c;

    while (1) {
        static struct option long_options[] = {
            /* These options set a flag. */
            {"name", required_argument, 0, 1},
            {"exec", required_argument, 0, 1},
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
            fprintf(stderr, "option -i\n");
            image_init();
            add_to_filters(image_filter, NULL);
            break;

        case 'l':
            add_to_printers(complete_printer, NULL);
            break;

        case 't':
            printf("option -t with value '%s'\n", optarg);
            break;

        case '?':
            /* getopt_long already printed an error message. */
            break;

        default:
            abort();
        }
    }

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        path = argv[optind++];
    }

    // Use default printer if nothing else was choosen
    if (wrapped_printers[0] == NULL) {
      add_to_printers(basic_printer, NULL);
      wrapped_printers[1] = NULL;
    }

    int ret = walk_from(path, wrapped_filters, wrapped_printers);
    image_close();

    exit(ret);
}
