#ifndef __COMMON_H__

#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <stdlib.h>

extern int readn(int sockfd, char* buffer, int size);

#endif
