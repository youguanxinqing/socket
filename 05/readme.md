应用层探活机制的实现

消息格式：
```c
typedef struct {
    u_int32_t type;
    char data[1024];
} msg_alive;
```
