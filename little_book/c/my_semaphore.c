#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_semaphore.h"

static void perror_exit (char *s)
{
    perror (s);  exit (-1);
}

static void *check_malloc(int size)
{
    void *p = malloc (size);
    if (p == NULL) perror_exit ("malloc failed");
    return p;
}

typedef pthread_mutex_t Mutex ;

static Mutex * make_mutex ()
{
    Mutex * mutex = check_malloc ( sizeof ( Mutex ));
    int n = pthread_mutex_init ( mutex , NULL );
    if ( n != 0) perror_exit (" make_lock failed ");
    return mutex ;
}

static void mutex_lock ( Mutex * mutex )
{
    int n = pthread_mutex_lock ( mutex );
    if ( n != 0) perror_exit (" lock failed ");
}

static void mutex_unlock ( Mutex * mutex )
{
    int n = pthread_mutex_unlock ( mutex );
    if ( n != 0) perror_exit (" unlock failed ");
}

typedef pthread_cond_t Cond ;

static Cond * make_cond ()
{
    Cond * cond = check_malloc ( sizeof ( Cond ));
    int n = pthread_cond_init ( cond , NULL );
    if (n != 0) perror_exit (" make_cond failed ");
    return cond ;
}

static void cond_wait ( Cond * cond , Mutex * mutex )
{
    int n = pthread_cond_wait ( cond , mutex );
    if (n != 0) perror_exit (" cond_wait failed ");
}

static void cond_signal ( Cond * cond )
{
    int n = pthread_cond_signal ( cond );
    if (n != 0) perror_exit (" cond_signal failed ");
}

int my_sem_init(struct my_sem_t *my_sem, unsigned int value)
{
    my_sem->value = value;
    my_sem->wakeups = 0;
    my_sem->mutex = make_mutex();
    my_sem->cond = make_cond();
    return 0;
}

int my_sem_post(struct my_sem_t *my_sem)
{
    mutex_lock(my_sem->mutex);
    my_sem->value++;
    if (my_sem->value <= 0)
    {
        my_sem->wakeups++;
        cond_signal(my_sem->cond);
    }
    mutex_unlock(my_sem->mutex);
    return 0;
}

int my_sem_wait(struct my_sem_t *my_sem)
{
    mutex_lock(my_sem->mutex);
    my_sem->value--;
    if (my_sem->value < 0)
    {
        do
        {
            cond_wait(my_sem->cond, my_sem->mutex);
        } while (my_sem->wakeups < 1);
        my_sem->wakeups--;
    }
    mutex_unlock(my_sem->mutex);
    return 0;
}
