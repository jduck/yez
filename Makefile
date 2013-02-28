CC = gcc

LS_DIR = libnsock
LS_CFLAGS = -I$(LS_DIR)
LS_LDFLAGS = -lnsock -L$(LS_DIR)

CFLAGS = -g -Wall -I. $(LS_CFLAGS)
LDFLAGS = $(LS_LDFLAGS)

LI_DIR = ../libircii
LI_CFLAGS = $(CFLAGS) -I$(LI_DIR) -DDEBUG
LI_LDFLAGS = $(LDFLAGS) -lircii -L$(LI_DIR)

all: yez yez+li

YSRCS = yez.c randcrap.c break_line.c hexdump.c wfc.c login.c
YLSRCS = yez+li.c randcrap.c break_line.c hexdump.c wfc.c \
       ircii_print.c login.c

yez: $(YSRCS)
	$(CC) $(CFLAGS) -o yez $(YSRCS) $(LDFLAGS)

yez+li: $(YLSRCS)
	$(CC) $(CFLAGS) $(LI_CFLAGS) -o yez+li $(YLSRCS) $(LI_LDFLAGS)
	
clean:
	rm -f *.o yez yez+li

install: all
	install -m 755 yez $(HOME)/vault/bin
	install -m 755 yez+li $(HOME)/vault/bin

libnsock.a: $(LS_DIR)/libnsock.a
