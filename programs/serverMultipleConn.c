#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int doprocessing (int sock);

int main( int argc, char *argv[] ) {
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n, pid;

    /* First call to socket() function */
    printf("sockfd = socket(AF_INET, SOCK_STREAM, 0)\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    printf("bind(sockfd, &serv_addr)\n");
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here
     * process will go in sleep mode and will wait
     * for the incoming connection
     */

    printf("listen(sockfd, MAX_CONN)\n");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    while (1) {
        printf("newsockfd = accept(sockfd, &client_addr)\n");
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        printf("New incoming connection\n");

        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        /* Create child process */
        pid = fork();

        if (pid < 0) {
            perror("ERROR on fork");
            exit(1);
        }

        if (pid) {
            /* This is the parent */
#if 1
            close(newsockfd);
#else
            close(sockfd);
            while(doprocessing(newsockfd));
            exit(0);
#endif
        } else {
            /* This is the child */
            close(sockfd);
            while(doprocessing(newsockfd));
            close(newsockfd);
            exit(0);
        }
    }
}


int doprocessing (int sock) {
    int n;
    char buffer[256];
    bzero(buffer,256);

    n = write(sock,"> ",2);
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    n = read(sock,buffer,255);
    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("pid:%d Message: %s\n",getpid(), buffer);


    if (buffer[0] == 'X')
        return 0;
    else 
        return 1;
}
