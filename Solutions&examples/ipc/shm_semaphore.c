#include <semaphore.h>  // all semaphore-related stuff
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <errno.h>      // errno
#include <pthread.h>    // posix-thread library
#include <sys/ipc.h>
#include <sys/shm.h>

#define CHILD_LIFETIME 3
#define SHM_KEY 0x2000

void child(void)
{
  int shmid;
  sem_t *sem;
  do {
    shmid = shmget(SHM_KEY, sizeof(sem_t), 0);
    sleep(1);
  } while((shmid < 0) && (errno == ENOENT));
  if (shmid < 0) {
    perror("shmget");
    exit(1); 
  }
  sem = shmat(shmid, NULL, 0);
  if (!sem) {
    perror("shmat");
    exit(1);
  }

  printf("Child: Acquiring semaphore...\n");
  sem_wait(sem);
  printf("Child: Done.\n");
  printf("Child: Terminating in %d seconds\n", CHILD_LIFETIME);
  sleep(CHILD_LIFETIME);
  sem_post(sem);
  printf("Child: Releasing semaphore\n");
  pthread_exit(NULL);
}

void parent(void)
{
  int status;
  char message[] = "Good day!";
  char *reply;
  int retval;
  int shmid;
  pid_t pid;
  sem_t *sem;

  shmid = shmget(SHM_KEY, sizeof(sem_t), IPC_CREAT|IPC_EXCL|0644);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }
  printf("Parent: Creating semaphore (with lock on)...\n");
  sem = shmat(shmid, NULL, 0);
  sem_init(sem, 1, 0); // Get semaphore, created already locked (value = 0)

  sleep(2);
  printf("Parent: Releasing semaphore...\n");
  sem_post(sem);
  sleep(1);
  printf("Parent: trying to acquire semaphore...\n");
  sem_wait(sem);
  printf("Parent: semaphore acquired again, exiting...\n");
  shmdt(sem);
  wait(&pid);
  shmctl(shmid, IPC_RMID, NULL);
  exit(0);
}

int main(void)
{
  if (fork() == 0)
    child();
  else
    parent();
}
