#ifndef __MQTT_H
#define __MQTT_H
#include "main.h"
#include "lwip_tcp_demo.h"

#define     BYTE0(temp)                        *((uint8_t *)(&temp))
#define     BYTE1(temp)                        *((uint8_t *)(&temp)+1)
#define     BYTE2(temp)                        *((uint8_t *)(&temp)+2)
#define     BYTE3(temp)                        *((uint8_t *)(&temp)+3)
/*
*   ÑÌÎí´«¸ÐÆ÷
//  "ProductKey": "a1HflwuvkIR",
//  "DeviceName": "iot_mq2",
//  "DeviceSecret": "eb59bb2d088a36d1fe44af6a9fd301d9"
*/
extern u8  mqtt_txbuf[256];
extern u16 mqtt_txlen;


extern uint8_t MQTT_Connect_Pack(u8 *mqtt_buff,u32 *mqtt_len);
extern void MQTT_SendHeart(void);
extern uint8_t MQTT_PublishData_Pack(char *topic ,char *message, uint8_t qos);
extern uint8_t MQTT_SubsrcibeTopic(char *topic,uint8_t qos,uint8_t whether);
#endif