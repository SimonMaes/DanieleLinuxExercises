#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int mkfifo(const char *pathname, mode_t mode);

#define FIFO_PATH "/tmp/fifo_example.fifo"

int main(void)
{
  char buffer_in[200], buffer_out[]="hello, child!";
  pid_t pid;

  int r, fifo_fd;

  if (mkfifo(FIFO_PATH, 0600) != 0) {
    perror("fifo");
    return -2;
  }

  pid = fork();

  if (pid < 0) return -2; /* fork error */

  if (pid > 0) {          /** Parent process **/
    char buffer_out[]="hello, child!";
    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd < 0) {
      perror("open");
      return -1;
    }
    printf("Parent: Writing message to the fifo\n");
    write(fifo_fd, buffer_out, strlen(buffer_out));
    printf("Parent: Waiting for child to terminate\n");
    wait();
    printf("Parent: Child terminated. Exiting...\n");
    close(fifo_fd);
    unlink(FIFO_PATH);
  } else {                /** Child process **/
    char buffer_in[200];
    printf("Child: opening fifo \n");
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd < 0) {
      perror("open");
      return -1;
    }
    printf("Child: reading... \n");
    r = read(fifo_fd, buffer_in, sizeof(buffer_in));
    if (r < 0) {
      perror("Child, read");
      return -3;
    }
    buffer_in[r] = (char)0; /* Terminate string */
    printf("Child: received string  '%s'\n", buffer_in);
    printf("Child: terminating\n");
    close(fifo_fd);
  }
  return 0;
}

