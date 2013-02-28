#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "wfc.h"

#include <nsock/nsock.h>

#ifdef NON_BLOCKING_CONNECTS
int
wait_for_connect(nsock_t *nst, time_t start)
{
   int connected = 0;
   time_t now;
   
   printf("waiting for connect...");
   fflush(stdout);
   while (!connected)
     {
	switch (nsock_is_connected(nst))
	  {
	   case 1:
	     connected = 1;
	     continue;
	   case -1:
	     nsock_close(nst);
	     return -1;
	  }
	usleep(100000);
	
	now = time(NULL);
	if ((now - start) >= CONNECT_TIMEOUT)
	  {
	     errno = ETIMEDOUT;
	     nsock_close(nst);
	     return -1;
	  }
	printf(".");
	fflush(stdout);
     }
   if (nsock_set_blocking(nst, 0) == -1)
     {
	perror("unable to set socket back to blocking");
	nsock_close(nst);
	return -1;
     }
   printf("connected!\n");
   return 1;
}
#endif
