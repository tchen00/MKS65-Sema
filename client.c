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
    sem1.sem_flg = SEM_UNDO;
    sem1.sem_num = 0;
    sem1.sem_op = -1;
    semop(sem_id, &sem1,1);

    int fd=open("story.txt",O_RDWR | O_APPEND,0644);

    if (fd == -1) {
      printf("Error while opening %s\n",strerror(errno));
      sem1.sem_op = 1;
      semop(sem_id, &sem1,1);
      return 0;
    }

    int shmid=shmget(KEY2,sizeof(int),IPC_CREAT | IPC_EXCL | 0644);
    char last_line[256];
    if (shmid==-1) {
        shmid=shmget(KEY2,sizeof(int),IPC_CREAT | 0644);
        count=shmat(shmid,0,0);
        lseek(fd,-1*(*count),SEEK_END);
        read(fd,last_line,*count);
        printf("Last line:\n%s",last_line);
    }
    else {
        printf("Last line: N/A\n");
        count=shmat(shmid,0,0);
    }
    char message[256];
    printf("What would you like the next line to be? ");
    fgets(message, 256, stdin);
    *count=strlen(message);
    shmdt(count);
    write(fd, message, strlen(message));
    close(fd);
    sem1.sem_op = 1;
    semop(sem_id, &sem1, 1);
    return 0;
}
