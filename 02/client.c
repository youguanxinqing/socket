#ifndef __CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include <errno.h>

#define MAXLINE 1024

#endif

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stdout, "Usage: udpclient [IP Address]\n");
        return 0;
    }
    
    int socketfd;
    struct sockaddr_in seraddr, reply_addr;
    socklen_t serlen = sizeof(seraddr), reply_len = sizeof(reply_addr);
    bzero(&seraddr, serlen);
    bzero(&reply_addr, reply_len);

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &seraddr.sin_addr);

    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error(1, errno, "socket error");
    }


    int len = 0, n = 0;
    char recv_line[MAXLINE+10] = {'\0'}, send_line[MAXLINE+1] = {'\0'};
    while (fgets(send_line, MAXLINE, stdin) != NULL) {
        len = strlen(send_line);
        if (send_line[len-1] == '\n') {
            send_line[len-1] = 0;
        }
        
        n = sendto(socketfd, send_line, len, 0, (struct sockaddr*) &seraddr, serlen);
        if (n < 0) {
            error(1, errno, "send error");
        }

        n = recvfrom(socketfd, recv_line, MAXLINE+10, 0, (struct sockaddr*) &reply_addr, &reply_len);
        if (n < 0) {
            error(1, errno, "recv error");
        }
        if (n < MAXLINE+10) {
            recv_line[n] = 0;
        } else {
            recv_line[n-1] = 0;
        }
        printf("%s\n", recv_line);
        
    }
}

