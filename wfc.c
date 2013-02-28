#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "nsock_tcp.h"

#ifdef NON_BLOCKING_CONNECTS
int
wait_for_connect(int s, time_t start)
{
   int connected = 0;
   time_t now;
   
   printf("waiting for connect...");
   fflush(stdout);
   while (!connected)
     {
	switch (nsock_tcp_connected(s))
	  {
	   case 1:
	     connected = 1;
	     continue;
	   case -1:
	     close(s);
	     return -1;
	  }
	usleep(500000);
	
	now = time(NULL);
	if ((now - start) >= CONNECT_TIMEOUT)
	  {
	     errno = ETIMEDOUT;
	     close(s);
	     return -1;
	  }
	printf(".");
	fflush(stdout);
     }
   if (nsock_tcp_set_blocking(s, 0) == -1)
     {
	perror("unable to set socket back to blocking");
	close(s);
	return -1;
     }
   printf("connected!\n");
   return 1;
}
#endif
