#ifndef __CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <bits/socket.h>
#include <arpa/inet.h>

#define MAXLINE 1024

#endif


int main(int argc, char** argv) {

    if (argc < 2) {
        error(1, 0, "Usage: c <IP Addr>");
    }

    int serfd = 0;
    struct sockaddr_in seraddr;
    int serlen = sizeof(seraddr);
    bzero(&seraddr, serlen);

    if ((serfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error(1, errno, "sockert error\n");
    }

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &seraddr.sin_addr);

    if (connect(serfd, (struct sockaddr*) &seraddr, serlen) < 0) {
        printf("connect error\n");
    }

    int n = 0;
    char recv_line[MAXLINE], send_line[MAXLINE];
    
    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);  // 监听标准输入
    FD_SET(serfd, &allreads);  // 监听套接字

    while (1) {
        readmask = allreads;
        int res = select(serfd+1, &readmask, NULL, NULL, NULL);
        if ( res <= 0 ) {
            error(1, errno, "select error");
        }

        if (FD_ISSET(serfd, &readmask)) { // 套接字可读写
            if ((n = recv(serfd, recv_line, MAXLINE, 0)) < 0) {
                error(1, errno, "recv error");
            } else if (n == 0) {
                error(1, 0, "server terminated");

            }

            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }

        if (FD_ISSET(0, &readmask)) { // 标准输入可读
            if (fgets(send_line, MAXLINE, stdin) != NULL) {
                int i = strlen(send_line);
                if (send_line[i-1] == '\n') {
                    send_line[i-1] = 0;
                }

                printf("send data: %s\n", send_line);
                if ((n = send(serfd, send_line, i, 0)) < 0) {
                    error(1, errno, "send error");
                }

                printf("send %d bytes\n", n);
            }

        }
    }

}

