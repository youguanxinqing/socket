#ifndef _SERVER_H_


#include "common.h"
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

#endif 


void read_data(int sockfd) {
    
    char buf[1024];
    
    int count = 0;
    while (1) {
        fprintf(stdout, "block in read data\n");
        ssize_t ret = readn(sockfd, buf, 1024);
        if (ret == 0) {
            fprintf(stdout, "read over\n");
            return;
        }
        count++;
        fprintf(stdout, "read %ld bytes, %d th\n", ret, count);
        
        usleep(1000); // simulate handling data
    }
}

int main() {
    
    int ret;
    int listenfd, connfd;
    struct sockaddr_in addr, cliaddr;

    bzero(&addr, sizeof(addr)); // clear
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        error(-1, errno, "socket error");
    }

    ret = bind(listenfd, (struct sockaddr*) &addr, sizeof(addr));
    if (ret < 0) {
        error(-1, errno, "bind error");
    }

    ret = listen(listenfd, 1024);
    if (ret < 0) {
        error(-1, errno, "listen error");
    }

    
    socklen_t clien = sizeof(cliaddr);
    while (1) {
        fprintf(stdout, "start accept\n");
        connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clien);
        if (connfd < 0) {
            error(-1, errno, "accept error");
        }
        read_data(connfd);
        close(connfd);
    }
    return 0;
}

