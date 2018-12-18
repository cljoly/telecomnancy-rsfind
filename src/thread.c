#include "thread.h"

void *filter_thread() {
  // TODO Fill this
  return NULL;
}

pthread_t *run_n_thread(int nb_thread) {
  pthread_t *ids = (pthread_t*)malloc(sizeof(pthread_t*)*nb_thread);

  for (int i = 0; i<nb_thread-1; i++) {
    pthread_create(&ids[i], NULL, filter_thread, NULL);
  }

  return ids;
}

void wait_threads(int *ids, int nb_thread) {
  /* for (int i = 0; i<nb_thread; i++) { */

  /* } */
  // TODO Fill this
  fprintf(stderr, "THREADS %i %i", ids[0], nb_thread);
  return;
}
