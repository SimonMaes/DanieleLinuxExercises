#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>


#define MAX_USER (80)
#define MAX_MESSAGE (1024)
#define MAX_CMD (1024)

static char username[MAX_USER];
static char message[MAX_MESSAGE];
static char reciever[MAX_USER];
static char path_reciever[14 + MAX_USER];
static char path_user[14 + MAX_USER];

static const char path_format[] = "/tmp/msg/%s.mbox";

static void mbox_create();
static void mbox_read(int outputfd);
static void mbox_write();
static void mbox_log();
static void cleanup();


int main()
{
  atexit(cleanup);
  mbox_create();
  mbox_read(STDIN_FILENO);

  for(;;)
    {
      mbox_write();
      mbox_log();
    }
}



static void mbox_create()
{
  printf("Welcome!\n");  
  printf("Enter your username:\n");
  scanf("%s",username);
  printf("You are currently logged in as: %s\n",username);					
  sprintf(path_user,path_format,username);				

  /* Make directory */
  if (mkdir("/tmp/msg",0777) < 0)
    {
      // perror( "Error making directory" );
    } 
}


static void mbox_log()
{
  char log[34+MAX_USER+MAX_USER+MAX_MESSAGE];
  int fd,r;
  fd = open("/tmp/msg/mailboxes.log", O_WRONLY | O_CREAT | O_APPEND, 0660 );

  if (fd < 0)
    {	
      perror( "Error opening log file" );
    }

  r = sprintf(log,"User: %s \t--> Reciever: %s \t Message: %s\n", username, reciever, message);
  write(fd, log, r);

  close(fd);
}



static void mbox_write()
{
  int fd,r;
  char init_message[15+MAX_USER];
  char cmd[MAX_CMD];

  struct pollfd pfd[1];

  printf("\nEnter your command in the format '/SEND <username> <msg>' or enter 'quit' to exit the application:\n");
  printf("CMD>");
  fflush(NULL); //If the stream argument is NULL,  fflush()  flushes  all  open output streams. 
 
  pfd[0].fd = STDIN_FILENO;
  pfd[0].events = POLLIN | POLLERR | POLLHUP;

  poll(pfd, 1, -1);
  
  if (pfd[0].revents &  (POLLHUP | POLLERR))
    {
      exit(0);
    }

  read(STDIN_FILENO, cmd, MAX_CMD);
  if( ! (sscanf(cmd, "/SEND %s %[ -z]", reciever, message) == 2))
    {
      if(strstr(cmd, "quit") != NULL)
	{
	  exit(0);
	}
      else
	{
	  printf("\nPlz try again..");
	}
    }


  sprintf(path_reciever,path_format,reciever);				

  fd = open(path_reciever, O_WRONLY | O_CREAT | O_APPEND, 0600);
  //O_WRONLY,.. is what you want to do with it now. The mode determines the permissions ONLY on the moment when the file is created! Trying to open the same file later on, with other permissions, has no effect!
  //Note that this mode(Permission Flags) only applies to future accesses of the newly created file; the open() call that creates a read-only file may well return a read/write file descriptor.

  if (fd < 0)
    {	
      perror( "Error opening file" );
    }

  r = snprintf(init_message,(sizeof(username)+15),"Message From %s:\n", username);
  write(fd, init_message, r);
  write(fd, message, sizeof(message));
  write(fd, "\n", 1);


  close(fd); // Necessary to close the file descriptor as soon as possible! so its necessary to open in each function a new file descriptor!
}


static void mbox_read(int outputfd)
{ 
  char buffer[1024];
 
  /* Open file */
  int fd = open(path_user, O_RDONLY); // Only reading from file, writing is to STDIN
  if (fd < 0)
    {	
      perror( "Error opening file" );
    }

  printf("This message was placed for you:\n");  

  /* Read/write data from file to STDIN_FILENO */
  for(;;)
    { 
      int r = read(fd, buffer, 1024);  
      if (r > 0)
	{
	  write(outputfd, buffer, r);
	}
      else
	break;
    }
  
  close(fd);

  /* Trunc file to 0 = zeroing the file */
  fd = open(path_user, O_WRONLY | O_CREAT | O_TRUNC , 0600); //The mode for the permissions has only effect where the file does not yet exist.
  if (fd < 0)
    {	
      perror( "Error truncing file to 0" );
    }
  else
    {
      close(fd);
    }
}
 
static void cleanup()
{
  printf("Cleaning up...\n");
}
