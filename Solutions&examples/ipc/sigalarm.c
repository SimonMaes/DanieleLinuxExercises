#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void signal_mgr(int signo)
{
  int status;
  printf("Signal %d received\n", signo);
  alarm(2);
}

int main(void)
{
  int r;
  sigset(SIGALRM, signal_mgr);
  alarm(2);
  while(1) {
    r = sleep(200);
    if (r != 0) 
     perror("sleep");
  }
}
