#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <errno.h>      // errno
#include <semaphore.h>  // all semaphore-related stuff
#include <pthread.h>    // posix-thread library

#define CHILD_LIFETIME 3

static sem_t sem;

void *child(void *arg)
{
  char *message = (char *) arg;
  printf("Child: Hello, msg is '%s'\n", message );
  printf("Child: Acquiring semaphore...\n");
  sem_wait(&sem);
  printf("Child: Done.\n");
  printf("Child: Terminating in %d seconds\n", CHILD_LIFETIME);
  sleep(CHILD_LIFETIME);
  sem_post(&sem);
  pthread_exit(NULL);
}

void parent(void)
{
  int status;
  char message[] = "Good day!";
  char *reply;
  int retval;

  pthread_t p_chld;

  sem_init(&sem, 0, 0); // Get semaphore, created already locked (value = 0)

  printf("Parent: Starting child with msg: %s\n", message);
  retval = pthread_create(&p_chld, NULL, child, message); 
  if (retval != 0) {
    printf("Error creating thread: %s\n", strerror(errno));
    exit(0);
  }
  sleep(1);
  printf("Parent: Releasing semaphore...\n");
  sem_post(&sem);
  sleep(1);
  printf("Parent: trying to acquire semaphore...\n");
  sem_wait(&sem);
  printf("Parent: semaphore acquired again, exiting...\n");

  exit(0);
}

int main(void)
{
  parent();
}
