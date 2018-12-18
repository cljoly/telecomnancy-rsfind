#include "image_search.h"
#include "list_dir.h"
#include "printers.h"
#include "name_search.h"
#include "pcre_search.h"
#include "textSearch.h"
#include "exec.h"
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

    // TODO Supporter plusieurs --name & cie (en l’état, le dernier prime,
    // puisqu’il écrase les valeurs précédentes)
    ///////values
    char name[DNAME_LENGTH];
    char ename[DNAME_LENGTH];
    // TODO Size of a script, find something better
    char exec[500];
    // TODO Size of a search query
    char search_str[500];
    char search_reg[500];
    // Number of thread
    int nb_thread;

    ///////getting opts
    int c;

    while (1) {
        static struct option long_options[] = {
            /* These options set a flag. */
            {"name", required_argument, 0, 0},
            {"exec", required_argument, 0, 0},
            {"ename", required_argument, 0, 0},
            {"listing_long", no_argument, 0, 'l'},
            {"chaine", required_argument, 0, 't'},
            {"regexp", required_argument, 0, 'T'},
            {"is_image", no_argument, 0, 'i'},
            {0, 0, 0, 0}};
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "iltpT:", long_options, &option_index);
        c = getopt_long(argc, argv, "iltp:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 0:
            /* If this option set a flag, do nothing else now. */
            // if (long_options[option_index].flag != 0)
            //  break;
            fprintf(stderr, "option %s, index %i", long_options[option_index].name,
                   option_index);
            if (optarg)
                fprintf(stderr, " with arg %s\n", optarg);
            if (option_index == 0) {
                strcpy(name, optarg);
                add_to_filters(name_filter, name);
            }
            if (option_index == 1) {
                strcpy(exec, optarg);
                add_to_printers(exec_printer, exec);
            }
            if (option_index == 2) {
              strcpy(ename, optarg);
              add_to_filters(ename_filter, ename);
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
          strcpy(search_str, optarg);
          add_to_filters(text_filter, search_str);
            break;

        case 'T':
          strcpy(search_reg, optarg);
          add_to_filters(pcre_filter, search_reg);
          break;

        case 'p':
          nb_thread = atoi(optarg);
          break;

        case '?':
            /* getopt_long already printed an error message. */
            fprintf(stderr, "getopt: '?' case\n");
            break;

        default:
            fprintf(stderr, "getopt: default case\n");
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

    int ret = walk_from(path, wrapped_filters, wrapped_printers, nb_thread);

    image_close();

    exit(ret);
}
