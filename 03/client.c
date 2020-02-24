#ifndef __CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <linux/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>

#define MAXLINE 1024

#endif

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: c [Local Path]\n");
        exit(-1);
    }
    char *local_path = argv[1];
    
    int clifd;
    struct sockaddr_un seraddr;
    int serlen = sizeof(seraddr);
    bzero(&seraddr, serlen);
    
    if ((clifd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
        error(1, errno, "socket error");
    }

    seraddr.sun_family = AF_LOCAL;
    strncpy(seraddr.sun_path, local_path, strlen(local_path));
    if (connect(clifd, (struct sockaddr*) &seraddr, serlen) < 0) {
        error(1, errno, "connect error");
    }

    int n, input_len = 0;
    char input[MAXLINE] = {0};
    char recv_line[MAXLINE + 5] = {0};
    while (fgets(input, MAXLINE, stdin) != NULL) {
        input_len = strlen(input);
        input[input_len-1] = 0; // delete '\n'

        n = write(clifd, input, input_len);
        if (n < 0) {
            error(1, errno, "send error");
        } else if (n == 0) {
            printf("conn break\n");
            break;
        }
        printf("send %d bytes: %s\n", n, input);


        n = read(clifd, recv_line, MAXLINE);
        if (n < 0) {
            error(1, errno, "recv error");
        } else if (n == MAXLINE+5) {
            /* recv_line[n-1] = 0; */
            puts("1");
        }
        printf("recv %d bytes: %s\n", n, recv_line);
    }
    
    close(clifd);
    return 0;
}

