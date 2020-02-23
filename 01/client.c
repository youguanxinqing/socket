#ifndef __CLIENT_H__

#include "common.h"
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <error.h>
#include <errno.h>

#endif

#define SIZE 10241 * 100



void send_data(int sockfd) {
    char data[SIZE+1];
    for (int i=0; i<SIZE; i++) {
        data[i] = 'a';
    }
    data[SIZE] = '\0';

    ssize_t len = write(sockfd, data, strlen(data));
    fprintf(stdout, "write %ld\n", len);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stdout, "usage: program [IP Address]\n");
        return -1;
    }
    
    struct sockaddr_in seraddr;
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &seraddr.sin_addr);
    
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = connect(socketfd, (struct sockaddr*) &seraddr, sizeof(seraddr));
    if (ret < 0) {
        error(-1, errno, "connect error");
    }

    send_data(socketfd);

    return 0;
}

