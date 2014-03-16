#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "commons.h"
#include "utils.h"

#define SRV_IP "127.0.0.1"

int main(void)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    unsigned int sendcount = 0;
    char buf[BUFLEN_C];
    time_t now, next = 0;


    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	diep("socket");

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    if (inet_aton(SRV_IP, &si_other.sin_addr) == 0) {
	fprintf(stderr, "inet_aton() failed\n");
	exit(1);
    }

    now = time(NULL);
    next = now + 5;
    while(1) {
	for (i = 0; i < NPACK; i++) {

	    now = time(NULL);
	    if(now >= next) {
		next = now + 5;
		printf("Sending packet %d. sendcount = %u\n", i, sendcount);
	    }
#ifdef SUP
	    memset(buf, 'A' + i, BUFLEN_C);
	    buf[BUFLEN_C-1] = '\0';
#endif
	    if (sendto(s, buf, BUFLEN_C, 0, (struct sockaddr *)&si_other, slen) == -1)
		diep("sendto()");
	    ++sendcount;
	}
//	getc(stdin);
    }

    close(s);
    return 0;
}
