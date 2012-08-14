#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <string.h>
#include <errno.h>

#define MAX_USER (80)
#define MAX_CMD  (1024)

static char login[MAX_USER] = "";
static const char filename_format[] = "/tmp/msg/%s.mbox";

struct mbox_msg {
  char sender[MAX_USER];
  char receiver[MAX_USER];
  char payload[MAX_CMD];
};


static void mbox_log(struct mbox_msg *msg)
{
  int fd = open("/tmp/msg/mbox.log", O_WRONLY| O_CREAT |O_APPEND, 0600);
  char newline = '\n';
  char separator = ':';
  char arrow[3] = "->";

  if (fd >= 0) {
    write(fd, msg->sender, strlen(msg->sender));
    write(fd, &arrow, 2);
    write(fd, msg->receiver, strlen(msg->receiver));
    write(fd, &separator, 1);
    write(fd, msg->payload, strlen(msg->payload));
    write(fd, &newline, 1);
    close(fd);
  }
}

static int mbox_send(struct mbox_msg *msg)
{
  char filename[MAX_USER + 20];
  int fd;
  char newline = '\n';
  char separator = ':';

  snprintf(filename, MAX_USER + 20, filename_format, msg->receiver);
  fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0600);
  if (fd < 0)
    return -1;
  (void)lseek(fd, 0, SEEK_END);
  write(fd, msg->sender, strlen(msg->sender));
  write(fd, &separator, 1);
  write(fd, msg->payload, strlen(msg->payload));
  write(fd, &newline, 1);
  close(fd);
  mbox_log(msg);
  free(msg);
}

static void mbox_show(int outfd)
{
  char filename[MAX_USER + 20];
  char buf[1024];
  int fd, r;
  snprintf(filename, MAX_USER + 20, filename_format, login);
  fd = open(filename, O_RDONLY);
  if (fd < 0)
    return;

  for(;;) {
    r = read(fd, buf, 1024);
    if (r > 0)
      write(outfd, buf, r);
    else
      break;
  }
  close(fd);
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC , 0600);
  if (fd >= 0)
    close(fd);
}

static void parse_login(char *str)
{
  char *end;
  end = strstr(str, " ");
  if (end)
    *end = '\0';
  strncpy(login,str,MAX_USER);
}

static int parse(char *str, int len)
{
  const char send[]="/SEND ";
  int strlen_send = strlen(send);
  char *usr, *msg, *end;
  struct mbox_msg *new;

  end = strstr(str, "\n");
  if (end)
    *end = '\0';

  if (!login[0]) {
    parse_login(str);
    return 0;
  }
  if (strncmp(send, str, strlen_send) != 0) {
    errno = EINVAL;
    return -1;
  }
  usr = str + strlen(send);
  end = strstr(usr, " ");
  if (!end)
    return 0; /* Empty msg */
  *end = '\0';
  msg = end + 1;
  new = malloc(sizeof(struct mbox_msg));
  if(!new)
    return -1;
  strncpy(new->sender,login,MAX_USER);
  strncpy(new->receiver,usr,MAX_USER);
  strncpy(new->payload,msg,MAX_CMD);
  return mbox_send(new);
}


int main(int argc, char *argv[])
{
  int prompt;
  char username[MAX_USER];
  char cmd[MAX_CMD];

  prompt = STDIN_FILENO;

  mkdir("/tmp/msg", 0700);

  /* Start Loop */
  for(;;) {
    int ret;
    struct pollfd pfd;
    int msg_shown = 0;

    pfd.fd = prompt;
    pfd.events = POLLIN | POLLERR | POLLHUP;

    /* Show Prompt... */
    if(!login[0]) {
      printf("Username: ");
    } else {
      if (!msg_shown) {
        msg_shown = 1;
        mbox_show(STDOUT_FILENO);
        printf("\n");
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
    ret = read(prompt, cmd, MAX_CMD);
    if (ret <= 0) {
      exit(0);
    }
    /* Ensure null-termination */
    cmd[MAX_CMD - 1] = '\0';
    /* Call parser */
    parse(cmd, ret);
  } /* loop again */
}
