#ifndef __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <error.h>
#include <errno.h>

#define MAXLINE 1024

#endif

static int count;

void program_over(int signo) {
    printf("\nrecv %d datagram\n", count);
    exit(0);
}


int main() {
    int sockfd;
    struct sockaddr_in cliaddr, seraddr;
    socklen_t clilen = sizeof(cliaddr), serlen = sizeof(seraddr);
    bzero(&cliaddr, clilen);
    bzero(&seraddr, serlen);

    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(12345);
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error(1, errno, "socket error");
    }

    if (bind(sockfd, (struct sockaddr*) &seraddr, serlen) < 0) {
        error(1, errno, "bind error");
    }

    signal(SIGINT, program_over);
    
    int n = 0;
    char recv_line[MAXLINE+1] = {'\0'}, send_line[MAXLINE+10] = {'\0'};
    while (1) {
        n = recvfrom(sockfd, recv_line, MAXLINE, 0, (struct sockaddr*) &cliaddr, &clilen);
        if (n < 0) {
            error(1, errno, "recv error");
        }
        recv_line[n] = 0;
        printf("%s\n", recv_line);
        
        sprintf(send_line, "Hi, %s", recv_line);
        n = sendto(sockfd, send_line, strlen(send_line), 0, (struct sockaddr*) &cliaddr, clilen);
        if (n < 0) {
            error(1, errno, "send error");
        }
        count++;
    }

    return 0;
}

