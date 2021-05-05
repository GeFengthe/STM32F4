#ifndef __LWIP_COMM_H
#define __LWIP_COMM_H
#include "stm32f4xx_hal.h"

#define LWIP_MAX_DHCP_TRIES		4   //DHCP服务器最大重试次数

typedef struct {
    uint8_t mac[6];
    uint8_t remoteip[4];
    uint8_t ip[4];
    uint8_t netmask[4];
    uint8_t gateway[4];
    volatile uint8_t dhcpstatus;
}__lwip_dev;

extern __lwip_dev lwipdev;	//lwip控制结构体

void lwip_pkt_handle(void);
void lwip_periodic_handle(void);

void lwip_comm_default_ip_set(__lwip_dev *lwipx);
uint8_t lwip_comm_mem_malloc(void);
void lwip_comm_mem_free(void);
uint8_t lwip_comm_init(void);
void lwip_dhcp_process_handle(void);

#endif

