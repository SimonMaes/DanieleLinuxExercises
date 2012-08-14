#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <errno.h>      // errno
#include <sys/file.h>
#include <sys/shm.h>

#define CHILD_LIFETIME 3
#define LOCKFILE "/tmp/lock_token.lck"

void child(void)
{
  int fd;
  do{
    fd = open(LOCKFILE, O_WRONLY);
    sleep(1);
  } while(fd < 0);

  printf("Child: Acquiring lock...\n");
  flock(fd, LOCK_EX);
  printf("Child: Done.\n");
  printf("Child: Terminating in %d seconds\n", CHILD_LIFETIME);
  sleep(CHILD_LIFETIME);
  flock(fd, LOCK_UN);
  printf("Child: Releasing lock\n");
  pthread_exit(NULL);
}

void parent(void)
{
  int status;
  char message[] = "Good day!";
  char *reply;
  int retval;
  int fd = open(LOCKFILE, O_WRONLY|O_CREAT|O_TRUNC,0600);
  pid_t pid;

  if (fd < 0) {
    perror("Error while opening lockfile");
    exit(1);
  }
  printf("Parent: Acquiring lock...\n");
  flock(fd, LOCK_EX);
  printf("Parent: Done.\n");
  sleep(2);
  printf("Parent: Releasing lock...\n");
  flock(fd, LOCK_UN);
  sleep(1);
  printf("Parent: trying to acquire lock...\n");
  flock(fd, LOCK_EX);
  printf("Parent: lock acquired again, exiting...\n");
  flock(fd, LOCK_EX);
  wait(&pid);
  close(fd);
  unlink(LOCKFILE);
  exit(0);
}

int main(void)
{
  if (fork() == 0)
    child();
  else
    parent();
}
