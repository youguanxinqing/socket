#ifndef __SERVER_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <error.h>
#include <errno.h>
#include <signal.h>

#define MAXLINE 1024

#endif

static int count;

void sig_int(int signo) {
    printf("\nrecv %d datagram\n", count);
    exit(0);
}

int main() {
    int sockfd, connfd; 
    struct sockaddr_in seraddr, cliaddr;
    socklen_t serlen = sizeof(seraddr), clilen = sizeof(clilen);
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

    if ((connfd = accept(sockfd, (struct sockaddr*) &cliaddr, &clilen)) < 0) {
        error(1, errno, "accept error");
    }

    signal(SIGINT, sig_int);
    signal(SIGPIPE, SIG_IGN);

    int n = 0;
    char recv_line[MAXLINE] = {0}, send_line[MAXLINE] = {0};
    count = 0;
    while (1) {
        n = read(connfd, recv_line, MAXLINE-1);
        if (n < 0) {
            error(1, errno, "read error");
        } else if (n == 0) {
            error(1, 0, "conn terminate");
        }
        
        recv_line[n] = 0;
        printf("recv %d bytes: %s\n", n, recv_line);
        count++;

        bzero(send_line, MAXLINE);
        snprintf(send_line, MAXLINE, "Hi, %s", recv_line);
        printf(">>> %s\n", send_line);

        sleep(5);

        n = write(connfd, send_line, strlen(send_line));
        printf("send %d bytes\n", n);
        if (n < 0) {
            error(1, errno, "write error");
        }
    }

    return 0;
}

