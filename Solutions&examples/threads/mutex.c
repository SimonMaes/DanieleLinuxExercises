#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <errno.h>      // errno
#include <pthread.h>    // posix-thread library

#define CHILD_LIFETIME 3

static pthread_mutex_t mux;

void *child(void *arg)
{
  char *message = (char *) arg;
  printf("Child: Hello, msg is '%s'\n", message );
  printf("Child: Acquiring mutex...\n");
  pthread_mutex_lock(&mux);
  printf("Child: Done.\n");
  printf("Child: Terminating in %d seconds\n", CHILD_LIFETIME);
  sleep(CHILD_LIFETIME);
  pthread_mutex_unlock(&mux);
  pthread_exit(NULL);
}

void parent(void)
{
  int status;
  char message[] = "Good day!";
  char *reply;
  int retval;

  pthread_t p_chld;

  pthread_mutex_init(&mux, NULL);
  pthread_mutex_lock(&mux);

  printf("Parent: Starting child with msg: %s\n", message);
  retval = pthread_create(&p_chld, NULL, child, message); 
  if (retval != 0) {
    printf("Error creating thread: %s\n", strerror(errno));
    exit(0);
  }
  sleep(1);
  printf("Parent: Releasing mutex...\n");
  pthread_mutex_unlock(&mux);
  sleep(1);
  printf("Parent: trying to acquire mutex...\n");
  pthread_mutex_lock(&mux);
  printf("Parent: mutex acquired again, exiting...\n");

  exit(0);
}

int main(void)
{
  parent();
}
