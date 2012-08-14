#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <errno.h>      // errno
#include <pthread.h>    // posix-thread library


#define CHILD_LIFETIME 3
static char child_reply[] = "Goodbye";

static __thread thread_specific;
static thread_generic;

void *child(void *arg)
{
  char *message = (char *) arg;
  printf("Child: Hello, msg is '%s'\n", message );
  printf("Child: Terminating in %d seconds\n", CHILD_LIFETIME);
  sleep(CHILD_LIFETIME);
  thread_generic = 142;
  thread_specific = 152;
  printf("Child: variable values: %d:%d\n", thread_generic, thread_specific);
  pthread_exit(child_reply);
}

void parent(void)
{
  int status;
  char message[] = "Good day!";
  char *reply;
  int retval;

  thread_generic = 42;
  thread_specific = 52;
  pthread_t p_chld;
  pthread_attr_t attr;
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, 0);

  printf("Parent: variable values: %d:%d\n", thread_generic, thread_specific);
  printf("Parent: Starting child with msg: %s\n", message);
  retval = pthread_create(&p_chld, &attr, child, message); 
  if (retval != 0) {
    printf("Error creating thread: %s\n", strerror(errno));
    exit(0);
  }
  pthread_join(p_chld, (void **)&reply);
  printf("Parent: Child returned saying '%s', Exiting...\n", reply);
  sleep(1);
  printf("Parent: variable values: %d:%d\n", thread_generic, thread_specific);
  exit(0);
}

int main(void)
{
  parent();
}
