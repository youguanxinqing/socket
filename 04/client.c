#ifndef __SERVER_H__

#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define MAXLINE 1024

#endif

int main(int argc, char** argv) {
    if (argc < 2) {
        error(1, errno, "Usage: c [IP Address]");
    }

    int sockfd;
    struct sockaddr_in seraddr;
    socklen_t serlen = sizeof(seraddr);
    bzero(&seraddr, serlen);

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &seraddr.sin_addr);

    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if (sockfd < 0){
        error(1, errno, "socket error");
    }
    
    if (connect(sockfd, (struct sockaddr*) &seraddr, serlen) < 0) {
        error(1, errno, "connect error");
    }

    int n;
    char send_line[MAXLINE] = {0}, recv_line[MAXLINE] = {0};
    
    fd_set readmask, allreads;

    FD_ZERO(&allreads);  // clear a set
    FD_SET(0, &allreads);  // add a given fd from a set
    FD_SET(sockfd, &allreads);
    while (1) {
        readmask = allreads;
        int rc = select(sockfd+1, &readmask, NULL, NULL, NULL);
        if (rc <= 0) {
            error(1, errno, "select error");
        }

        if (FD_ISSET(sockfd, &readmask)) {
            n = read(sockfd, recv_line, MAXLINE-1);
            if (n < 0) {
                error(1, errno, "read error");
            } else if (n == 0) {
                error(1, errno, "connect terminated");
            }
            
            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }

        if (FD_ISSET(0, &readmask)) {
            if (fgets(send_line, MAXLINE-1, stdin) != NULL) {
                if (strncmp(send_line, "shutdown", 8) == 0) {
                    FD_CLR(0, &allreads);
                    if (shutdown(sockfd, SHUT_WR)) {
                        error(1, errno, "shutdown error");
                    }
                } else if (strncmp(send_line, "close", 5) == 0) {
                    FD_CLR(0, &allreads);
                    if (close(sockfd)) {
                        error(1, errno, "close error");
                    }
                    sleep(6);
                    exit(0);  // send FIN packgae
                } else {
                    int i = strlen(send_line);
                    if (send_line[i-1] == '\n') {
                        send_line[i-1] = 0;
                    }
                    printf("now sending: %s\n", send_line);
                    n = write(sockfd, send_line, i);
                    if (n < 0) {
                        error(1, errno, "send error");
                    }
                    printf("send bytes: %d\n", n);
                }
            }
        }
    }

    return 0;
}

