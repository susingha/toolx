#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "commons.h"
#include "utils.h"

int main(void)
{
    struct sockaddr_in si_me, si_other;
    int s, slen=sizeof(si_other);
    unsigned int recvcount = 0;
    unsigned long bw = 0;
    unsigned int i = 0;
    char buf[BUFLEN_S];
    time_t start, now, next = 0;
    long long int bytes = 0;

#ifdef TIMEDEBUG
    while(1) {
	now = time(NULL);
	printf("time: %u\n", now);
	sleep(1);
    }
#endif

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	diep("socket");

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me))==-1)
	diep("bind");

    recvfrom(s, buf, BUFLEN_S, 0, (struct sockaddr *)&si_other, &slen);
    printf("\nServer started receiving data\n\n");;
    start = now = time(NULL);
    next = now + 5;
    while(1) {
//  for (i=0; i<NPACK; i++) {
	if (recvfrom(s, buf, BUFLEN_S, 0, (struct sockaddr *)&si_other, &slen)==-1)
	    diep("recvfrom()");
	buf[BUFLEN_S-1] = '\0';
	recvcount++;
	bytes += BUFLEN_S;

	now = time(NULL);
	if(now >= next) {
	    next = now + 5;

#if 1
	    printf("Received packet from %s:%d\n%lld Bytes. bw = %.2f Bytes/s\n\n",
		    inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port),
		    bytes, ((float)bytes/(float)(now - start)));
#else
	    printf("Received packet from %s:%d\nrecvcount = %u. bw = %f Kbits/s\n\n",
		    inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port),
		    /*buf,*/ recvcount, (float)(recvcount * ((BUFLEN_S / 1024)) * 8)/(float)(now - start));
#endif
	}
//	sleep(1);
//  }
    }

    close(s);
    return 0;
}
