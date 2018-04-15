#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


int a=0;

void * f(void *p){
     pthread_mutex_lock(&mtx);

     for(i=0;i<100000;i++) 
	 a++;
     
     pthread_mutex_unlock(&mtx);

     return p;
}

int main(){
     pthread_t t1,t2;
     pthread_create(&t1,NULL,f,NULL);
     pthread_create(&t2,NULL,f,NULL);
     pthread_join(t1,NULL);
     pthread_join(t2,NULL);
     printf("a=%d\n",a);
}

