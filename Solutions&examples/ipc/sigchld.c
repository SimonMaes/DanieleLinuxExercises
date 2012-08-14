#include <signal.h>
#include <stdio.h>

void child_mgr(int signo)
{
  int status;
  printf("Signal %d received\n", signo);
  wait(&status);
}

int main(void)
{
  sigset(SIGCHLD, child_mgr);
  if (fork() == 0) {
    sleep(2);
    return(0);
  }
  sleep(3600);
  sleep(2);
}
