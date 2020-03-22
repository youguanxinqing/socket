#ifndef __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <error.h>
#include <errno.h>

#define MAXLINE 1024

#endif

int main() {
    int sockfd, connfd;
    struct sockaddr_in seraddr, cliaddr;
    socklen_t serlen = sizeof(seraddr), clilen = sizeof(cliaddr);
    bzero(&seraddr, serlen);
    bzero(&cliaddr, clilen);

    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(12345);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error(1, errno, "socket error");
    }

    if (bind(sockfd, (struct sockaddr*) &seraddr, serlen) < 0) {
        error(1, errno, "bind error");
    }

    if (listen(sockfd, 1024) < 0) {
        error(1, errno, "listen error");
    }

    if ((connfd=accept(sockfd, (struct sockaddr*)&cliaddr, &clilen)) < 0) {
        error(1, errno, "accpet error");
    }

    int n = 0;
    char recv_line[MAXLINE] = {0}, send_line[MAXLINE + 10] = {0};
    while(1) {
        n = recv(connfd, recv_line, MAXLINE-1, 0);
        if (n < 0) {
            error(1, errno, "recv error");
        } else if (n == 0) {
            printf("conn terminate\n");
            break;
        }

        sprintf(send_line, "Hi, %s", recv_line);
        n = send(connfd, send_line, strlen(send_line), 0);
        if (n < 0) {
            error(1, errno, "send error");
        }
    }

    return 0;
}

