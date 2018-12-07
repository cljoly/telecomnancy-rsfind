#include <pthread.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#ifndef LIST_DIR_H
#define LIST_DIR_H

pthread_t *run_n_thread(int nb_thread);
void wait_threads(int *ids, int nb_thread);

#endif
