
#include <string.h>

#include "break_line.h"

int
break_line(char **ip, char *l, int ml)
{
   char *cr, *lf;

   if (!*ip || !**ip)
     return 0;
   
   cr = strchr(*ip, '\r');
   lf = strchr(*ip, '\n');
   if ((cr && lf && lf - cr == 1) || lf)
     {
	if (cr)
	  *cr = '\0';
	*lf++ = '\0';
	
	strncpy(l, *ip, ml-1);
	l[ml-1] = '\0';
	*ip = lf;
	return 1;
     }
   return 0;
}
