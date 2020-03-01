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
#include <time.h>

#include "common.h"

#endif

static int count;

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: s [sleepngtime]");
        exit(0);
    }
    
    int sleeping_time = atoi(argv[1]);

    int listenfd, connfd;
    struct sockaddr_in seraddr, cliaddr;
    socklen_t serlen = sizeof(seraddr), clilen = sizeof(cliaddr);
    bzero(&seraddr, serlen);
    bzero(&cliaddr, clilen);

    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(12345);
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        error(1, errno, "socket error");
    }

    if (bind(listenfd, (struct sockaddr*) &seraddr, serlen) < 0) {
        error(1, errno, "bind error");
    }

    if (listen(listenfd, 1024) < 0) {
        error(1, errno, "listen error");
    }

    connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clilen);
    if (connfd < 0) {
        error(1, errno, "connect error");
    }

    int n = 0;
    alive_pack pack;
    count = 0;
    while (1) {
        n = read(connfd, (char*) &pack, sizeof(pack));
        if (n < 0) {
            error(1, errno, "read error");
        } else if (n == 0) {
            error(1, errno, "client closed");
        }

        printf("recv %d bytes\n", n);
        count++;

        switch(ntohl(pack.type)) {
        case MSG_PING : {
            alive_pack echo_pack;
            echo_pack.type = htonl(MSG_PONG);
            sleep(sleeping_time);
            n = send(connfd, (char*) &echo_pack, sizeof(echo_pack), 0);
            if (n < 0) {
                error(1, errno, "send error");
            }
            break;
        }
        case MSG_TYPE1 : printf("process type1\n"); break;
        case MSG_TYPE2 : printf("process type2\n"); break;
        default:
            error(1, 0, "unknown msg type (%d)\n", ntohl(pack.type));
        }
    }

    return 0;
}

