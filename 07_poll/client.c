#ifndef __CLIENT_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 1024

#endif 


int main(int argc, char** argv) {
    
    if (argc < 2) {
        error(1, errno, "Usag: c <IP Addr>");
    }
    
    int sockfd;
    struct sockaddr_in seraddr;
    int serlen = sizeof(seraddr);
    bzero(&seraddr, serlen);

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &seraddr.sin_addr);
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error(1, errno, "socket error");
    }
    
    if (connect(sockfd, (struct sockaddr*) &seraddr, serlen) < 0) {
        error(1, errno, "connect error");
    }

    int n = 0;
    char buf[MAXLINE] = {0};
    while (1) {
        if (fgets(buf, MAXLINE, stdin) != NULL) {
            n = write(sockfd, buf, strlen(buf));
            if (n < 0) {
                error(1, errno, "write error");
            }

            n = read(sockfd, buf, MAXLINE);
            if (n < 0) {
                error(1, errno, "read error");
            }
            buf[n] = 0;
            printf("read data: %s\n", buf);
        }
    }


    return 0;
}

