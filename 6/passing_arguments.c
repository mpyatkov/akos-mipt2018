#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct test{
    int pos;
    int *arr;
};

void *fill_array( void *arg) {
    struct test *t = (struct test *) arg;
    
    printf("thread -> %d\n", t->pos);

    for (int i = t->pos; i<t->pos+5; ++i)
    	t->arr[i] = i*i;
    
    return (void *)(t ->pos);
}

int main() {
    int *a = (int *) calloc(10, sizeof(int));

    void *ret1;
    void *ret2;

    pthread_t t1,t2;

    struct test s1 = {.pos = 0,
		      .arr = a};
    
    struct test s2 = {.pos = 5,
		      .arr = a};
    
    pthread_create(&t1, NULL, &fill_array, &s1);
    pthread_create(&t2, NULL, &fill_array, &s2);

    pthread_join(t1, &ret1);
    pthread_join(t2, &ret2);

    printf("x=%d, y=%d\n", (int)ret1, (int)ret2);
    
    for (int i = 0; i<10;i++)
    	printf("a[%d] = %d\n", i, a[i]);

    free(a);
    return 0;
}
