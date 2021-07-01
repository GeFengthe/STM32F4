#ifndef __LWIP_TCP_DEMO_H
#define __LWIP_TCP_DEMO_H
#include "main.h"
#include "lwip_comm.h"

#define TCP_CLIENT_RX_BUFSIZE               1000        //接收缓冲区
#define REMOTE_PORT                         1883        //定义远端主机IP地址
#define LWIP_SEND_DATA                      0X80        //定义有数据发送

#define FALSE                               1
#define TRUE                                0

extern u8 mqtt_alive;

extern void mqtt_sendheart(void);
extern uint8_t tcp_client_flag; 
extern void Sky_TcpClientThread_Init(void);
extern void lwip_tcpClient_send(u8 * sendbuf,u32 length);
#endif

