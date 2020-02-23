#include "common.h"

int readn(int sockfd, char* buffer, int size) {
    
    char* cursor = buffer;
    int length = size;
    
    while (length > 0) {
        int ret = read(sockfd, cursor, length);
        if (ret < 0) {
            if (errno == EXIT_FAILURE)
                error(-1, errno, "readn error");
            else 
                continue;
        } else if (ret == 0) {
            break;
        }

        cursor += ret;
        length -= ret;
    }
    return size - length; // readl read bytes
}

