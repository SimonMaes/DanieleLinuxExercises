#include <time.h>
#include <stdio.h>
 
int main(void)
{
   struct tm newtime;
   time_t ltime;
   char buf[50];
 
   ltime=time(&ltime);
   localtime_r(&ltime, &newtime);
   printf("The date and time is %s", asctime_r(&newtime, buf));
}
 
