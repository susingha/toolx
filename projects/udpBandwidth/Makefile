#
# $Id: Makefile,v 1.18 2004/11/01 16:22:03 mkirchner Exp $
#

CCFLAGS=-MD
CC=gcc

all: serverUDP clientUDP

serverUDP: serverUDP.o utils.o
	$(CC) $(LDFLAGS) serverUDP.o utils.o -o serverUDP
clientUDP: clientUDP.o utils.o
	$(CC) $(LDFLAGS) clientUDP.o utils.o -o clientUDP

serverUDP.o:
	$(CC) $(CCFLAGS) -c serverUDP.c
clientUDP.o:
	$(CC) $(CCFLAGS) -c clientUDP.c
utils.o:
	$(CC) $(CCFLAGS) -c utils.c



clean:
	rm -f serverUDP clientUDP *.o

