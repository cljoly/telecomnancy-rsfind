#include "printers.h"

void basic_printer(context *ctxt, char file[]) {
    if (ctxt == NULL) {
        printf("%s\n", file);
    } else {
        printf("%s/%s\n", ctxt->dir_name, file);
    }
}

void complete_printer(context *ctxt, char file[]) {
    // TODO Implement this
    printf("%s/%s\n", ctxt->dir_name, file);
    return;
}
