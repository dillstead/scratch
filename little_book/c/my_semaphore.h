#ifndef MY_SEMAPHORE
#define MY_SEMAPHORE

#include <pthread.h>

struct my_sem_t
{
    int value;
    int wakeups;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
};

int my_sem_init(struct my_sem_t *my_sem, unsigned int value);
int my_sem_post(struct my_sem_t *my_sem);
int my_sem_wait(struct my_sem_t *my_sem);
    
#endif
