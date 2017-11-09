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

// #define SRV_IP "172.24.245.113"
#define SRV_IP "127.0.0.1"

int main(int argc, char * argv[])
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    unsigned int sendcount = 0;
    char buf[BUFLEN_C];
    int * head = (int *)buf;
    time_t now, next = 0;
    char * srv_ip = argv[1];

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	diep("socket");

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    if (inet_aton(srv_ip, &si_other.sin_addr) == 0) {
	fprintf(stderr, "inet_aton() failed\n");
	exit(1);
    }

    now = time(NULL);
    next = now + 5;
    while(1) {
	for (i = 0; i < NPACK; i++) {

	    ++sendcount;

	    now = time(NULL);
	    if(now >= next) {
		next = now + 5;
		printf("Sending packet %d. sendcount = %u\n", i, sendcount);
	    }
#ifdef SUP
	    memset(buf, 'A' + i, BUFLEN_C);
	    *head = sendcount;
	    buf[BUFLEN_C-1] = '\0';
#endif
	    if (sendto(s, buf, BUFLEN_C, 0, (struct sockaddr *)&si_other, slen) == -1)
		diep("sendto()");
	}
//	getc(stdin);
    }

    close(s);
    return 0;
}
