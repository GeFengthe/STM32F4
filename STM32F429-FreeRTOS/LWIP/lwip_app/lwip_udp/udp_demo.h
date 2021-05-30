#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H
#include "main.h"

#define UDP_DEMO_RX_BUFSIZE         2000                    //定义udp最大接收数据长度
#define UDP_DEMO_PORT               8089                    //定义udp连接的本地端口号
#define LWIP_SEND_DATA              0x80                    //定义有数据发送

extern uint8_t udp_flag;  
extern void udp_demo_init(void);
#endif