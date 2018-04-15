/* Пример с уменьшением размера стека, */
/* для того, чтобы увеличить количество потоков */
/* в ограниченном объеме памяти */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
//ulimit -v 64000

void* hello(void* a) {
    printf("Started\n");
    sleep(5);
    printf("Ended\n");
    pthread_exit(0);
}

int main(int argc, char **argv) {
    int N = 100;
    pthread_t threads[100];

    pthread_attr_t attr;
    int *p;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr,100000);
    
    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], &attr, hello, NULL);
        /* pthread_create(&threads[i], NULL, hello, NULL); */
    }
 
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_attr_destroy(&attr);

    return 0;
}

