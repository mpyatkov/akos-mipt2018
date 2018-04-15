#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

/*
  Отображение файла в память
  strace ./a.out не покажет ниодного вызова write
  тем не менее файл test1 будет изменен
 */

int main(int argc, char *argv[])
{
    int fd = open("test1", O_RDWR, 0);
    void *fm = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    char *data = fm;
    // по умолчанию в файле test1: BBBBBBBB
    sprintf(data, "%s", "ATGC");
 
    munmap(fm, 4096);
}
