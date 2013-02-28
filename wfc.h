#ifndef __WFC_H_
#define __WFC_H_

#define CONNECT_TIMEOUT 10

#ifdef NON_BLOCKING_CONNECTS
int wait_for_connect(nsock_t *nst, time_t start);
#endif

#endif
