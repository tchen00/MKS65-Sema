#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>

#define KEY 1989
#define KEY2 271

void create(){
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short * array;
        struct seminfo * __buf;
    } semaphore;
   printf("You have chosen to create the story\n");
   int sem_id = semget(KEY,1,IPC_CREAT | IPC_EXCL | 0600);
      if (sem_id >= 0) {
          semaphore.val = 1;
          semctl(sem_id, 0, SETVAL, semaphore.val);
          int fd=open("story.txt", O_CREAT | O_TRUNC,0644);
          if (fd == -1){
              printf("Error in opening: %s\n", strerror(errno));
              return;
          }
          close(fd);
      }
      else{
          printf("Already created. Try running ./client or ./control -v\n");
      }
}

void view(){
    printf("You have chosen to VIEW the story:\n");
    int fd = open("story.txt", O_RDONLY);
    if (fd == -1){
        printf("Error in opening: %s\n", strerror(errno));
        return;
    }
    char * file = calloc(2056, sizeof(char));
    int last = read(fd, file, 2056);
    if (last == -1){
        printf("Error in reading: %s\n", strerror(errno));
        return;
    }
    char* text = strrchr(file, '\n');
    if (text){
       *text = 0;
     }
    printf("%s\n", file);
    free(file);
    close(fd);
    return;
}

void delete(){
    printf("You have chosen to remove the story\n");
    view();
    remove("story.txt");
    int shmid = shmget(KEY2, sizeof(int), 0);
    int what = shmctl(shmid, IPC_RMID, NULL);
    int semid = semget(KEY, 1, 0);
    semctl(semid, 0, IPC_RMID);
    printf("Removed!\n");
}
