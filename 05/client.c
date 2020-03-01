#ifndef __CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <error.h>
#include <errno.h>

#include "common.h"

#endif

int main(int argc, char** argv) {

    if (argc < 2) {
        fputs("Usage: c [IP Address]\n", stdout);
        exit(0);
    }
    
    int sockfd;
    struct sockaddr_in seraddr;
    socklen_t serlen = sizeof(seraddr);
    bzero(&seraddr, serlen);
    
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &seraddr.sin_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error(1, errno, "socket error");
    }
    
    if (connect(sockfd, (struct sockaddr*) &seraddr, serlen) < 0) {
        error(1, errno, "connect error");
    }

    int n = 0;
    char recv_line[MAXLINE + 1] = {0};
    
    fd_set readmask;
    fd_set allreads;
    
    struct timeval tv;
    int heartbeats = 0;

    tv.tv_sec = KEEP_ALIVE_TIME;
    tv.tv_usec = 0;

    alive_pack pack;
    
    FD_ZERO(&allreads);
    FD_SET(sockfd, &allreads);
    while (1) {
        readmask = allreads;
        // 每次 select 之后，tv 会被置空，需要再次赋值
        int ret = select(sockfd+1, &readmask, NULL, NULL, &tv);
        if (ret < 0) {
            error(1, errno, "select error");
        } else if (ret == 0) {
            if (++heartbeats > KEEP_ALIVE_PROBETIMES) {
                error(1, errno, "connect dead");
            }

            printf("sending heartbeat #%d\n", heartbeats);
            pack.type = htonl(MSG_PING);
            n = send(sockfd, (char*) &pack, sizeof(pack), 0);
            if (n < 0) {
                error(1, errno, "send error");
            }

            tv.tv_sec = KEEP_ALIVE_INTERVAL;
            continue;
        }

        if (FD_ISSET(sockfd, &readmask)) {
            n = recv(sockfd, recv_line, MAXLINE, 0);
            if (n < 0) {
                error(1, errno, "read error");
            } else if (n == 0) {
                error(1, errno, "server terminated");
            }

            printf("recv hearbeats, make heartbeat to 0\n");
            heartbeats = 0;
            tv.tv_sec = KEEP_ALIVE_TIME;
        }
    }
    
    return 0;
}

