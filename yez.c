
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "nsock_tcp.h"
#include "break_line.h"
#include "wfc.h"
#include "hexdump.h"
#include "login.h"

void talk(int s);

extern char *randcrap(int len);

int
main(c, v)
   int c;
   char *v[];
{
   int s;
#ifdef NON_BLOCKING_CONNECTS
   int wfc;
#endif
   char *from = NULL, *to = NULL, *pass = NULL;
   char ebuf[512];
   time_t start, end;
   nsocktcp_t *nst;
   
   /* check out the args */
   if (c < 2)
     {
	printf("usage: %s <host>[:<port>] [<from>[:<port>] [<password>]]\n", 
	       v[0]);
	return 1;
     }
   to = v[1];
   if (c > 2)
     from = v[2];
   if (c > 3)
     pass = v[3];
   
   /* check for a port.. */
   if (!strchr(to, ':'))
     to = nsock_tcp_host(v[1], 6667);
   
   /* initialize the nsock stuff.. */
   if (!(nst = nsock_tcp_init_connect(from, to,
#ifdef NON_BLOCKING_CONNECTS
				      NSTCP_NON_BLOCK,
#else
				      0,
#endif
				      ebuf, sizeof(ebuf))))
     {
	fprintf(stderr, "connect_init failed: %s\n", ebuf);
	return 1;
     }
   
   /* try to connect */
   start = time(NULL);
   s = nsock_tcp_connect(nst, 0);
   if (s < 1)
     {
	fprintf(stderr, "connect failed: %s\n", ebuf);
	return 1;
     }
#ifdef NON_BLOCKING_CONNECTS
   wfc = wait_for_connect(s, start);
   if (wfc == -1)
     {
	printf("Unable to connect to %s: %s\n", to, strerror(errno));
	return 1;
     }
#endif	
   end = time(NULL);
   printf("Connection to %s successful on socket #%u (%lu secs).\n", to, s, end-start);
   
   if (pass && *pass)
     login(s, pass);
   else
     login(s, NULL);
   printf("I think i'm logged in..\n");
   talk(s);
   close(s);
   return 0;
}

void
talk(s)
   int s;
{
   int sret, done = 0;
   char ibuf[1024], obuf[1024], aline[1024], *ip = ibuf;
   int il = 0, ol = 0, lr = 0;
   fd_set r, w;
   int so = fileno(stdout), si = fileno(stdin);

   memset(ibuf, 0, sizeof(ibuf));
   memset(obuf, 0, sizeof(obuf));
   while (!done)
     {
#ifdef DEBUG2
	printf ("obuf (%d):\n", ol);
	if (ol > 0)
	  hexdump(obuf, ol);
#endif
	FD_ZERO(&r);
	if (il < sizeof(ibuf)-1)
	   FD_SET(s, &r);
	if (ol < sizeof(obuf)-1)
	   FD_SET(si, &r);

	FD_ZERO(&w);
	if (ol > 0)
	   FD_SET(s, &w);
	if (il > 0)
	   FD_SET(so, &w);
	
#ifdef DEBUG1
	fprintf (stderr, 
		 "r: (s: %d, si: %d), w: (s: %d, so: %d)\n",
		 il < sizeof(ibuf)-1, ol < sizeof(obuf)-1, 
		 ol > 0, il > 0);
#endif	
	if ((sret = select(s+1, &r, &w, NULL, NULL)) == -1)
	  {
	     perror("select() is unhappy");
	     done = 1;
	     continue;
	  }
	
	if (!sret)
	  continue;

	if (FD_ISSET(so, &w))
	  {
	     ip = ibuf;
	     while (break_line(&ip, aline, sizeof(aline)))
	       {
		  if (strncmp(aline, "PING ", 5) == 0)
		    {
		       char tmp[128];
		       int len;
		       memset(tmp, 0, sizeof(tmp));
		       snprintf(tmp, sizeof(tmp)-1, "PONG%s\n", strchr(aline, ' '));
		       len = strlen(tmp);
		       write(s, tmp, len);
		    }
		  printf("%s\n", aline);
	       }
	     lr = strlen(ip);
	     strncpy(ibuf, ip, lr);
	     memset(ibuf+lr, 0, sizeof(ibuf)-lr);
	     il -= (ip - ibuf);
	     
	     if (!(--sret))
	       continue;
	  }

	if (FD_ISSET(s, &w))
	  {
	     lr = write(s, obuf, ol);
	     if (lr != ol)
	       {
		  printf("output buffer not empty..\n");
		  done = 1;
		  continue;
	       }
	     ol = 0;
	     memset(obuf, 0, sizeof(obuf));
	     if (!(--sret))
	       continue;
	  }

	if (FD_ISSET(s, &r))
	  {
	     lr = read(s, ibuf+il, sizeof(ibuf)-il-1);
	     if (lr == -1)
	       {
		  perror("read() is unhappy");
		  done = 1;
		  continue;
	       }
	     if (lr == 0)
	       {
		  printf("connection closed by server..\n");
		  done = 1;
		  continue;
	       }
	     il += lr;
	     if (!(--sret))
	       continue;
	  }

	if (FD_ISSET(si, &r))
	  {
	     lr = read(si, obuf+ol, sizeof(obuf)-ol-1);
	     if (lr < 1)
	       {
		  printf("\nEOF\n");
		  done = 1;
		  continue;
	       }
	     ol += lr;
	     if (!(--sret))
	       continue;
	  }
     }
}
