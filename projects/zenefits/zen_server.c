#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <string.h>

#include "zen_types.h"
#include "zen_server.h"
#include "zen_trie.h"

int doprocessing (int sock);
int register_handle_sigchld();
int sockfd_global = 0, newsockfd_global = 0;


int main( int argc, char *argv[] ) {
    int sockfd, newsockfd, portno;
    unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n, pid;

    // register a SIGCHLD signal handler
    register_handle_sigchld();

    // First call to socket() function
    printf("sockfd = socket(AF_INET, SOCK_STREAM, 0)\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    sockfd_global = sockfd;

    // Initialize socket structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = SERVERPORT;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Now bind the host address using bind() call
    printf("bind(sockfd, &serv_addr)\n");
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding to server port");
        exit(1);
    }

    /* Now start listening for the clients, here
     * process will go in sleep mode and will wait
     * for the incoming connection
     */

    printf("listen(sockfd, MAX_CONN) on %d\n", SERVERPORT);

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* Before we start accepting requests
     * Initialize the trie
     */
    zen_trie_init();

    while (1) {
	if (debug || debugnetwork)
	    printf("newsockfd = accept(sockfd, &client_addr)\n");

        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;
        }

	if (debug || debugnetwork)
	    printf("New incoming connection\n");

        // Create child process
        pid = fork();

        if (pid < 0) {
            perror("ERROR on fork");
	    close(newsockfd);
            continue;
        }

        if (pid) {
            // This is the parent
            close(newsockfd);
        } else {
            // This is the child
            close(sockfd);

	    // prepare for signal handling in case of ^C
	    newsockfd_global = newsockfd;
	    register_handle_sigchld();

	    // process traffic
            while(doprocessing(newsockfd));

	    // process connection closure
            close(newsockfd);
            exit(0);
        }
    }
}

int doprocessing (int sock) {
    int n;
    char query[QUERY_MAX_BUFFER];
    bzero(query, QUERY_MAX_BUFFER);

#if 0
    n = write(sock,"> ",2);
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
#endif

    // Read request string
    n = read(sock, query, QUERY_MAX_BUFFER - 1);
    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    if (debug) printf("pid:%d Message: %s/(%d, %lu)\n", getpid(), query, n, strlen(query));

    // Serve results:
#if 1
    zen_write_results(query, n, sock);
#else
    n = write(sock, "result >\n", 9);
#endif

    if (debugnetwork) {
	printf("sleep 2 before conn close\n");
	sleep (2);
    }
    return 0;
}

void handle_sigchld(int sig) {
    int status;

    printf("\n");

    if (sig == SIGCHLD) {
	wait(&status);
    }

    if (sig == SIGINT) {
	
	printf("releasing server port\n");

	if (newsockfd_global) {
	    shutdown(newsockfd_global, SHUT_RDWR);
	    close(newsockfd_global);
	}
	if (sockfd_global) {
	    shutdown(sockfd_global, SHUT_RDWR);
	    close(sockfd_global);
	}

	exit(0);
    }
}


int register_handle_sigchld()
{
    if (signal(SIGCHLD, handle_sigchld) == SIG_ERR) {
	perror("ERROR: Could not register signal handler. Expect zombies");
	return -1;
    }

    if (signal(SIGINT,  handle_sigchld) == SIG_ERR) {
	perror("ERROR: Could not register signal handler. Expect server port in use");
	return -2;
    }

    return 0;
}

int zen_get_write_socket()
{
    return newsockfd_global;
}
