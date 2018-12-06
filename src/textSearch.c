#include "textSearch.h"

int text_search(char* filepath, char toFind[255]) {
    int textSize = strlen(toFind);
    int textDetect[textSize];
    for (int i=0; i<textSize; i++) {
        textDetect[i]=0;
    }
    int pos_pointers = 0;
    int b=0;
    int c_reconnu=0;

    char c;
    int fd = open(filepath, O_RDONLY, 0);

    while((read(fd, &c, 1) != 0) && !b){ //Parcours le fichier
        //printf("%d\n",c_reconnu);
        c_reconnu = 0;
        for (int i=0; i<textSize; i++) { //Parcours le texte à trouver
            if (c==toFind[i]){
                /* printf("%d\n",i); */
                c_reconnu = 1;
                if (i == 0){ //Nouveau début possible
                    if (textSize == 1) { b=1;}
                    textDetect[pos_pointers] = 1;
                    (pos_pointers == textSize)? pos_pointers=0:pos_pointers++;
                }
                else {  //Incrémentation des pointeurs de recherche
                    for (int j = 0; j<textSize; j++){
                        if (textDetect[j] == i) {
                            (i==textSize-1)? b=1:textDetect[j]++; //Si le compteur à incrémenter est à la fin du mot on accepte
                        }
                    }
                }
            }
        }
        if (c_reconnu == 0){ //Caractère non reconnu => réinitialisation
            for (int i=0; i<textSize; i++) {
                   textDetect[i]=0;
            }
            pos_pointers=0;
        }
    }
    close(fd);
    return b;
}

filter_result text_filter(context *ctxt, char *path, int is_folder, char *extra_argument) {
  if (is_folder) {
      return FILTER_CONTINUE;
  }
  char filename[DNAME_LENGTH];
  complete_path(ctxt, path, filename);
  int b = text_search(filename, extra_argument);
  if (b == 0) {
    return FILTER_IGNORE;
  } else {
    return FILTER_KEEP;
  }
}
