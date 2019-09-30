#ifndef _PTHREAD_H
#include <pthread/pthread.h>

extern int __pthread_mutex_lock (pthread_mutex_t *__mutex);
extern int __pthread_mutex_unlock (pthread_mutex_t *__mutex);

#endif
