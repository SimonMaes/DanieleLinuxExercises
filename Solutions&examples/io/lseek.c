#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define OFFSET (2 * 1024 * 1024 - 1) /* 2 MB */

int main(void)
{

  int fd = open("sparse_file.tmp", O_WRONLY|O_CREAT|O_TRUNC, 0644);
  char c = 0;

  (void)lseek(fd, OFFSET, SEEK_SET);

  write(fd, &c, 1); 
  close(fd);
  return 0;

}

