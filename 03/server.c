#ifndef __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/un.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <strings.h>

#define MAXLINE 1024

#endif

int main(int argc, char** argv) {
    if (argc < 2) {
        fputs("Usage: s [Local Path]\n", stdout);
        exit(-1);
    }

    int listenfd, connfd;
    struct sockaddr_un cliaddr, seraddr;
    socklen_t clilen = sizeof(cliaddr), serlen = sizeof(seraddr);
    bzero(&cliaddr, clilen);
    bzero(&seraddr, serlen);
    
    char *local_path = argv[1];
    unlink(local_path);
    seraddr.sun_family = AF_LOCAL;
    strncpy(seraddr.sun_path, local_path, strlen(local_path));

    if ((listenfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
        error(1, errno, "socket error");
    }

    if (bind(listenfd, (struct sockaddr*) &seraddr, serlen) < 0) {
        error(1, errno, "bind error");
    }

    if (listen(listenfd, 1024) < 0) {
        error(1, errno, "listen error");
    }

    if ((connfd = accept(listenfd, (struct sockaddr*) &seraddr, &serlen)) < 0) {
        error(1, errno, "accept error\n");
    }
    
    int n;
    char data[MAXLINE] = {0};
    while(1) {
        n = read(connfd, data, MAXLINE);
        if (n < 0) {
            error(1, errno, "read error");
        } else if (n == 0) {
            fputs("conn break\n", stdout);
            break;
        } else if (n == MAXLINE) {
            data[n-1] = 0;
        }
        printf("recv %d bytes: %s\n", n, data);
        
        char send_line[MAXLINE + 5] = "Hi, ";
        strncat(send_line, data, n);
        n = write(connfd, send_line, strlen(send_line));
        if (n < 0) {
            error(1, errno, "write error");
        }
        printf("send %d bytes: %s\n", n, send_line);

    }
    
    close(connfd);
    close(listenfd);
    return 0;
}

