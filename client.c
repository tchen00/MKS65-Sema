#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define KEY 1989
#define KEY2 271

int main() {
    int sem_id=semget(KEY,1,0644);
    int * count;
    struct sembuf sem1;
