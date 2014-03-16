#
# $Id: Makefile,v 1.18 2004/11/01 16:22:03 mkirchner Exp $
#

# CCFLAGS=-Wall
CC=gcc

all:
	$(CC) -o serverUDP $(CCFLAGS) serverUDP.c  utils.c
	$(CC) -o clientUDP $(CCFLAGS) clientUDP.c  utils.c

clean:
	rm -f serverUDP clientUDP *.o

