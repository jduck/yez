#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern char *randcrap(int);

void
login(s, pw)
   int s;
   char *pw;
{
   char tbuf[1024];

   if (pw)
     {
	snprintf(tbuf, sizeof(tbuf)-1, "PASS %s\n", pw);
	tbuf[sizeof(tbuf)-1] = '\0';
	if (write(s, tbuf, strlen(tbuf)) != strlen(tbuf))
	  {
	     perror("short write on password");
	     return;
	  }
     }
   /* nickname */
   snprintf(tbuf, sizeof(tbuf)-1, "NICK %s\n", randcrap(9));
   tbuf[sizeof(tbuf)-1] = '\0';
   if (write(s, tbuf, strlen(tbuf)) != strlen(tbuf))
     {
	perror("short write on nickname");
	return;
     }
   
   /* user line */
   snprintf(tbuf, sizeof(tbuf)-1, "USER t3ztd00d %s +i :",
	    randcrap(10));
   strncat(tbuf, randcrap(128), sizeof(tbuf)-strlen(tbuf)-2);
   strcat(tbuf, "\n"); /* safe */
   if (write(s, tbuf, strlen(tbuf)) != strlen(tbuf))
     {
	perror("short write on user line");
	return;
     }
}
