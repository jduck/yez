#ifdef TEST
# include <stdio.h>
#endif

#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * return len of random isalpha()s
 */
char *
randcrap(len)
   int len;
{
   static char rstr[4096];
#if defined(TEST) || defined(IDENTD)
   char all[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
#else
   char all[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`\\_-^[]{}|";
#endif
   int i, t;
   long int r;
   float alen = strlen(all);
   struct timeval tv;
   unsigned long long usecs;

   memset(rstr, 0, sizeof(rstr));
   gettimeofday(&tv, NULL);
   usecs = (tv.tv_sec * 1000000) + tv.tv_usec;
   srandom(usecs / rand());
   r = usecs+random();
   srand(r*getpid());

   for (i = 0; i < len && i < sizeof(rstr)-1; i++)
     {
       t = (int)(alen * rand() / (RAND_MAX+1.0));
       rstr[i] = all[t];
     }
   rstr[i] = '\0';
   return rstr;
}

#ifdef TEST
int
main(int c, char *v[])
{
   int tmp;
#ifndef GOOD_TEST
   if (c >= 2)
     {
	tmp = atoi(v[1]);
	if (tmp > 0)
	  printf("%s", randcrap(tmp));
     }
#else
   for (tmp = 0; tmp < 50; tmp++)
     printf("%s\n", randcrap(9));
#endif
   return 0;
}
#endif
