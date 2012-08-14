#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
   int pipefd[2], pid, r;
   char buffer_in[200], buffer_out[]="hello, child!";

   if (pipe(pipefd) < 0) return -1; /* pipe error */
   pid = fork();

   if (pid < 0) return -2; /* fork error */

   if (pid > 0) {          /** Parent process **/
    printf("Parent: closing recv pipe end\n");
    close(pipefd[0]);
    printf("Parent: Writing message to the pipe\n");
    write(pipefd[1], buffer_out, strlen(buffer_out));
    printf("Parent: Waiting for child to terminate\n");
    wait();
    printf("Parent: Child terminated. Exiting...\n");


   } else {                /** Child process **/
     printf("Child: closing send pipe end\n");
     close(pipefd[1]);
     r = read(pipefd[0], buffer_in, 200);
     buffer_in[r] = (char)0; /* Terminate string */
     printf("Child: received string  '%s'\n", buffer_in);
     printf("Child: terminating\n");
   }
   return 0;
}
