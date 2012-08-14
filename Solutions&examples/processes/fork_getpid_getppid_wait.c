#include <unistd.h>     // fork(), getpid()
#include <sys/types.h>  // wait(), getpid()
#include <wait.h>       // wait()
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()

#define CHILD_LIFETIME 3

void child(void)
{
  pid_t pid, ppid;
  pid = getpid();
  ppid = getppid();
  printf("Child: Hello, pid=%d, ppid=%d\n", pid, ppid);
  printf("Child: Terminating in %d seconds\n", CHILD_LIFETIME);
  sleep(CHILD_LIFETIME);
  exit(0);
}

void parent(pid_t cpid)
{
  pid_t pid, wpid;
  int status;
  pid = getpid();
  printf("Parent: Hello, pid=%d, cpid=%d\n", pid, cpid);
  wpid = wait(&status);
  printf("Parent: Child %d returned, exiting...\n", wpid);
  exit(0);
}

int main(void)
{
  pid_t res = fork();
  if (res < 0) {
    perror("fork"); 
  }

  if (res > 0)
    parent(res);
  else
    child();
}
