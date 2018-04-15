#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

#define PHILOSOPHERS_COUNT 5

pthread_t philosopher[PHILOSOPHERS_COUNT];
pthread_mutex_t forks[PHILOSOPHERS_COUNT];
/* pthread_mutex_t officiant = PTHREAD_MUTEX_INITIALIZER; */

void take_forks(int n) {
     pthread_mutex_lock(&forks[min(n,(n+1)%5)]);
     pthread_mutex_lock(&forks[max(n,(n+1)%5)]);
     /* pthread_mutex_lock(&forks[n]); */
     /* pthread_mutex_lock(&forks[(n+1)%5]); */
}

void put_forks(int n) {
     pthread_mutex_unlock(&forks[(n+1)%5]);
     pthread_mutex_unlock(&forks[n]);
}

/* void officiant_on() { */
/*      pthread_mutex_lock(&officiant); */
/* } */

/* void officiant_off() { */
/*      pthread_mutex_unlock(&officiant); */
/* } */

void eating(int n) {
     printf ("Philosopher %d is eating\n",n);
     /* usleep(5000); */
}

void thinking(int n) {
     printf ("Philosopher %d is thinking\n",n);
     /* usleep(5000); */
}

void *philosopher_process(int n)
{
     while (1) {
	  thinking(n);
	  /* officiant_on(); */
	  take_forks(n);
	  eating(n);
	  put_forks(n);
	  /* officiant_off(); */
	  printf ("Philosopher %d finished eating\n",n);
     }

     return(NULL);
}

int main()
{
     int i;
     for(i=0;i<PHILOSOPHERS_COUNT;i++)
	  pthread_mutex_init(&forks[i],NULL);

     for(i=0;i<PHILOSOPHERS_COUNT;i++)
	  pthread_create(&philosopher[i], NULL, (void *)philosopher_process, (void *)i);

     for(i=0;i<PHILOSOPHERS_COUNT;i++)
	  pthread_join(philosopher[i],NULL);

     for(i=0;i<PHILOSOPHERS_COUNT;i++)
	  pthread_mutex_destroy(&forks[i]);

     /* pthread_mutex_destroy(&officiant); */

     return 0;
}
