#include <unistd.h> // execl()
#include <stdio.h>  // printf()
#include <sys/types.h>  // wait()
#include <wait.h>       // wait()
#include <stdlib.h>     // exit()

int main(void)
{
  char arg0[3] = "ls";
  char arg1[3] = "-l";
  char arg2[8] = "--color";
  char arg3[2] = ".";


  pid_t wpid;
  int status;

  if (fork() == 0) { // child
    execl("/bin/ls", arg0, arg1, arg2, arg3, (char *)NULL);
    /* The line above never returns, so the exit() here is just to avoid a warning */
    exit(0);
  }
  /* Parent */
  printf("Command launched, waiting to terminate...\n");
  wpid = wait(&status);
  printf("Wait returned, pid= %d\n", wpid);
  exit(0);


}
