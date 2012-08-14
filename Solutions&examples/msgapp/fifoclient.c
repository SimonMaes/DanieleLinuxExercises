#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>

#define MAX_USER (80)
#define MAX_CMD  (1024)

static char login[MAX_USER] = "";
static const char fifo_format[] = "/tmp/msg/%s.fifo";

struct mbox_msg {
  char sender[MAX_USER];
  char receiver[MAX_USER];
  char payload[MAX_CMD];
  time_t timestamp;
};


#define LOGIN_FIFO "/tmp/msg/login.fifo"

int main(int argc, char *argv[])
{
  int prompt, login_fifo, cmd_fifo;
  char username[MAX_USER];
  char cmd[MAX_CMD];
  int ret;
  struct pollfd pfd;
  int logged_in = 0;
  char fifoname[80];
  char *end;
  prompt = STDIN_FILENO;

  login_fifo = open(LOGIN_FIFO, O_WRONLY );
  if (login_fifo < 0) {
    fprintf(stderr, "Cannot connect to server. Is server running? (error: %s)\n", strerror(errno));
    exit(1);
  }

  /* Start Loop */
  for(;;) {

    pfd.fd = prompt;
    pfd.events = POLLIN | POLLERR | POLLHUP;

    /* Show Prompt... */
    if(!login[0]) {
      printf("Username: ");
    } else {
      if (!logged_in) {
        logged_in = 1;
        snprintf(fifoname, 80, fifo_format, login);
        printf("Opening CMD fifo...\n");
        do {
          cmd_fifo = open(fifoname, O_RDWR);
          usleep(100000);
        } while (cmd_fifo < 0);
        printf("CMD fifo open!\n");
        pfd.fd = cmd_fifo;
        while (poll(&pfd, 1, 1000) > 0) {
          ret = read(cmd_fifo, cmd, MAX_CMD);
          if (ret <= 0) {
            exit(0);
          }
          write(STDOUT_FILENO, cmd, ret);
        }
        continue;
      }
      printf("CMD> ");
    }
    fflush(NULL);

    /* Poll for input... */
    ret = poll(&pfd, 1, -1);
    if (ret == -1) {
      if (errno == EINTR)
        continue;
      else {
        perror("main loop");
        exit(errno);
      }
    }
    if (pfd.revents & (POLLHUP | POLLERR)) {
      exit(0);
    }
    if (pfd.revents & POLLIN) {
      ret = read(prompt, cmd, MAX_CMD);
      if (ret <= 0) {
        exit(0);
      }
      if (!login[0]) {
        printf("Logging in...\n");
        write(login_fifo, cmd, ret);
        end = strstr(cmd, "\n");
        if (end)
          *end = 0;
        strncpy(login, cmd, MAX_USER);
        printf("Logged in as %s\n", login);
      } else {
        write(cmd_fifo, cmd, ret);
      }
      continue;
    }
  } /* loop again */
}
