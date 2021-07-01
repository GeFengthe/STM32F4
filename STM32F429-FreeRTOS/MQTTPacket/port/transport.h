#ifndef __TRANSPORT_H
#define __TRANSPORT_H
#include "main.h"


extern int32_t transport_sendPacketBuffer(uint8_t* buf,uint32_t buflen);
extern int32_t transport_getdata(uint8_t* buf, int32_t count);
extern int32_t transport_open(int8_t* servip, int32_t port);
extern int transport_close(void);
#endif