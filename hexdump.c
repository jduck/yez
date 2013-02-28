#ifdef DEBUG2

#include <stdio.h>

#define MAX_HEX_WIDTH 40

void
hexdump(char *str, int len)
{
   int i, w;
   char *ch;
   
   if (!len)
     return;
   w = 0;
   for (i = 0, ch = str; i < len; i++, ch++)
     {
	printf("%02X", *ch);
	if (w == MAX_HEX_WIDTH)
	  {
	     printf("\n");
	     w = 0;
	  }
	else
	  {
	     printf(" ");
	     w++;
	  }
     }
   printf("\n");
}

#endif
