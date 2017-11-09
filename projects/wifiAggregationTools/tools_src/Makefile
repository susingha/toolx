#
# $Id: Makefile,v 1.18 2004/11/01 16:22:03 mkirchner Exp $
#

# CCFLAGS=-Wall
# CC=gcc

all: serverUDP clientUDP serverSCTP clientSCTP

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


serverSCTP: serverSCTP.o
	$(CC) -lsctp serverSCTP.o -o serverSCTP
clientSCTP: clientSCTP.o
	$(CC) -lsctp clientSCTP.o -o clientSCTP
serverSCTP.o:
	$(CC) $(CCFLAGS) -c serverSCTP.c
clientSCTP.o:
	$(CC) $(CCFLAGS) -c clientSCTP.c


clean:
	rm -f serverUDP clientUDP serverSCTP clientSCTP *.o

