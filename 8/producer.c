/* задача потребителя производителя */
/* sem_open,shm_open, unrelated processes */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

#define BLOCK_MEM_MUTEX "/sem-mutex"
#define ACTIVATE_CONSUMER "/sem-consumer"
#define ACTIVATE_PRODUCER "/sem-producer"
#define SHARED_MEM "/mem-prod-cons"

void error (char *msg)
{
    perror (msg);
    exit (1);
}

char buf[256];

int main (int argc, char **argv)
{
    char *data;
    sem_t *block_mem_mutex, *activate_consumer, *activate_producer;
    int fd_shm;
    char mybuf[256];
    
    if ((block_mem_mutex = sem_open (BLOCK_MEM_MUTEX, 0, 0)) == SEM_FAILED)
        error ("block_mem_mutex");
    
    if ((fd_shm = shm_open (SHARED_MEM, O_RDWR, 0)) == -1)
        error ("SHARED_MEM");

    if ((data = mmap (NULL, sizeof (buf), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0)) == MAP_FAILED)
       error ("mmap");
    
    if ((activate_consumer = sem_open (ACTIVATE_CONSUMER, 0)) == SEM_FAILED)
        error ("activate_consumer");

    if ((activate_producer = sem_open (ACTIVATE_PRODUCER, 0)) == SEM_FAILED)
        error ("activate_consumer");

    int index = 0;
    while (1) {
 
	sem_wait (activate_producer);
	sem_wait (block_mem_mutex);
	sprintf(data, "message: %d-%d\n", getpid(), index);
	printf(data, "message: %d-%d\n", getpid(), index);
	index++;
	usleep(100000);
	sem_post (block_mem_mutex);
	sem_post (activate_consumer);
	
    }
    
    
}
