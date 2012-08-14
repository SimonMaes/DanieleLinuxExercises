#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

#define KEY 0xc01a

struct memory_content
{
  unsigned long a, b, c;
  double pi, e;
  char signature[20];
  pid_t creator;
  pid_t last_access;
};

void set_values(struct memory_content *m)
{
  pid_t pid = getpid();
  m->a = 1;
  m->b = 2;
  m->c = 3;
  m->pi = 3.1415;
  m->e  = 2.7128; 
  m->creator = pid;
  m->last_access = pid;
  snprintf(m->signature, 20, "Tass Belgium");
}

void print_values(struct memory_content *m)
{
  printf("%d, %d, %d.... pi is %lf, e is %lf [%s]\nCreated by %d, last access by %d\n",
    m->a, m->b, m->c, m->pi, m->e, m->signature, m->creator, m->last_access);
}

int main(void)
{
  int res, creating = 0;
  struct memory_content *mem;

  res = shmget(KEY, sizeof(struct memory_content), 0);

  if ((res < 0) && (errno == ENOENT)) {
    printf("Memory area does not exist. Creating...\n");
    res = shmget(KEY, sizeof(struct memory_content), IPC_CREAT | IPC_EXCL | 0644);
    if (res < 0) {
      perror("shmget");
      exit(1);
    }
    creating = 1;
  }

  mem = shmat(res, NULL, 0);
  if (!mem) {
    perror("shmat");
    exit(1);
  }

  if (creating) {
    printf("Setting values...\n");
    set_values(mem);
  }

  print_values(mem);
  mem->last_access = getpid();
  shmdt(mem);
  return 0;
}
