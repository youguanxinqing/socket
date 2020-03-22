#ifndef __SERVER_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>
#include <bits/poll.h>
#include <sys/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <strings.h>

#define MAXLINE 1024
#define INIT_SIZE 1024
#define POLLRDNORM 0x040

#endif 


int main() {
    
    int lisfd, connfd;
    struct sockaddr_in seraddr, cliaddr;
    int serlen = sizeof(seraddr), clilen = sizeof(cliaddr);
    bzero(&seraddr, serlen);
    bzero(&cliaddr, clilen);

    if ((lisfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error(1, errno, "socket error");
    }

    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(12345);
    if (bind(lisfd, (struct sockaddr*) &seraddr, serlen) < 0) {
        error(1, errno, "bind error");
    }

    if (listen(lisfd, 1024) < 0) {
        error(1, errno, "listen error");
    }

    struct pollfd event_set[INIT_SIZE];
    // 用负值表示没有 io 监听
    for (int i=0; i<INIT_SIZE; ++i) {
        event_set[i].fd = -1;
    }

    // 监听套接字
    event_set[0].fd = lisfd;
    event_set[0].events = POLLRDNORM;

    int n;
    char buf[MAXLINE] = {0};
    
    int i = 0;
    int ready_number = 0;
    while (1) {
        ready_number = poll(event_set, INIT_SIZE, -1);
        if (ready_number < 0) {
            error(1, errno, "poll error");
        }

        if (event_set[0].revents & POLLRDNORM) {
            connfd = accept(lisfd, (struct sockaddr*) &cliaddr, (socklen_t *) &clilen);
               
            for (i=1; i<INIT_SIZE; ++i) {
                if (event_set[i].fd < 0) {
                    event_set[i].fd = connfd;
                    event_set[i].events = POLLRDNORM;
                    break;
                }
            }

            if (i == INIT_SIZE ) {
                error(1, errno, "allow max %d io(s)", INIT_SIZE);
            }
            
            if (--ready_number <= 0) {
                // 没有需要处理的 io 事件
                continue;
            }
        }

        for (i=1; i<INIT_SIZE; ++i) {
            int sockfd;
            if ((sockfd=event_set[i].fd) < 0) {
                continue;
            }

            if (event_set[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(sockfd, buf, MAXLINE)) > 0) {
                    if (write(sockfd, buf, n) < 0) {
                        error(1, errno, "write error");
                    }
                } else if (n == 0 || errno == ECONNRESET) {
                    close(sockfd);
                    event_set[i].fd = -1;
                } else {
                    error(1, errno, "read error");
                }

                if (--ready_number <= 0) {
                    break;
                }
            }
        }
    }


    return 0;
}

