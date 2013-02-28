
#include <stdio.h>
#include <string.h>

#include "libircii.h"

/*
 * print out the contents of an ircII struct
 */
void
ircii_print(ircII i)
{
   char tb[12], tb2[4096];
   int uh = 0;
   
   memset(tb, 0, sizeof(tb));
   switch (i.type)
     {
      case IRCII_TM_NUMERIC:
	sprintf(tb, "NUMERIC: %03d", i.numeric);
	break;
      default:
	if (i.type >= 0 && i.type < IRCII_TM_END)
	  strcpy(tb, ircii_str[i.type]);
	else
	  printf("Unknown type: 0x%x\n", i.type);
	break;
     }
   
   memset(tb2, 0, sizeof(tb2));
   if (i.user && i.host)
     uh = 1;
   snprintf(tb2, sizeof(tb2)-1, "%-12.12s%s%s%s%s%s%s%s%s%s%s%s", tb,
	    i.from ? " from " : "", i.from ? i.from : "",
	    uh ? " (" : "", uh ? i.user : "", uh ? "@" : "", uh ? i.host : "", uh ? ")" : "",
	    i.targ ? " to " : "", i.targ ? i.targ : "",
	    i.msg ? ": " : "", i.msg ? i.msg : "");
   puts(tb2);
}
