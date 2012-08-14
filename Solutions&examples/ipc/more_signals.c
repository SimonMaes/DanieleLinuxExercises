#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void signal_mgr(int signo)
{
  int status;
  printf("Signal %d received\n", signo);
  if (signo == SIGINT) {
    printf("SIGINT (ctrl + c) received, exiting... \n");
    exit(0);
  }
}

int main(void)
{
  int i, r;
  printf("Started with pid: %d\n", getpid());
  for (i = 1; i< 32; i++)
    sigset(i, signal_mgr);
  while(1) {
    r = sleep(240);
    if (r != 0) 
     perror("sleep");
  }
}
