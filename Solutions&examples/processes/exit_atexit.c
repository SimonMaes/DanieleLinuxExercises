#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()

void cleanup(void)
{
  printf("Exiting, cleaning up...\n");
  sleep(1);
  printf("Done\n");
}

int main(void)
{
  atexit(cleanup);
  printf("Terminating in 2 seconds...\n");
  sleep(2);
  printf("Calling exit...\n");
  exit(0);
}
