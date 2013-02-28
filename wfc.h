#ifndef __WFC_H_
#define __WFC_H_

#ifdef NON_BLOCKING_CONNECTS
int wait_for_connect(int s, time_t start);
#endif

#endif
