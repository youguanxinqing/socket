#ifndef __COMMON_H__

#include <sys/types.h>

#define MAXLINE 1024 * 4

#define KEEP_ALIVE_TIME 10 // 保活时间
#define KEEP_ALIVE_INTERVAL 3  // 探活间隔时间
#define KEEP_ALIVE_PROBETIMES 3  // 最大探活次数


#define MSG_PING 1
#define MSG_PONG 2
#define MSG_TYPE1 11
#define MSG_TYPE2 21

#endif


typedef struct {
    u_int32_t type;
    char data[1024];
} alive_pack;

