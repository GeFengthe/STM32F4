#ifndef __MQTT_H
#define __MQTT_H
#include "main.h"

#define   MSG_MAX_LEN     500
#define   MSG_TOPIC_LEN   50
#define   KEEPLIVE_TIME   300
#define   MQTT_VERSION    4

#if    LWIP_DNS
#define   HOST_NAME       "a1MZHjK2SCF.iot-as-mqtt.cn-shanghai.aliyuncs.com"     //服务器域名
#else
#define   HOST_NAME       "47.103.184.125"     //服务器IP地址
#endif


#define   HOST_PORT     1883    //由于是TCP连接，端口必须是1883


#define   CLIENT_ID     "00001|securemode=3,signmethod=hmacsha1|"         //
#define   USER_NAME     "STM32-light&a1delcKh1To"     //用户名
#define   PASSWORD      "420E5CF7FBD5D275D31ADBA73EFEE2D1DC272C1B"  //秘钥

#define   PRINT_DEBUG   printf

#define   TOPIC         "/a1delcKh1To/STM32-light/user/lighton_off"

enum QoS 
{ QOS0 = 0, 
  QOS1, 
  QOS2 
};

enum MQTT_Connect
{
  Connect_OK = 0,
  Connect_NOK,
  Connect_NOTACK
};

//数据交互结构体
typedef struct __MQTTMessage
{
    uint32_t qos;
    uint8_t retained;
    uint8_t dup;
    uint16_t id;
	uint8_t type;
    void *payload;
    int32_t payloadlen;
}MQTTMessage;

//用户接收消息结构体
typedef struct __MQTT_MSG
{
	  uint8_t  msgqos;                 //消息质量
	  uint8_t  msg[MSG_MAX_LEN];       //消息
	  uint32_t msglenth;               //消息长度
	  uint8_t  topic[MSG_TOPIC_LEN];   //主题    
	  uint16_t packetid;               //消息ID
	  uint8_t  valid;                  //标明消息是否有效
}MQTT_USER_MSG;

//发送消息结构体
typedef struct
{
    int8_t topic[MSG_TOPIC_LEN];
    int8_t qos;
    int8_t retained;

    uint8_t msg[MSG_MAX_LEN];
    uint8_t msglen;
} mqtt_recv_msg_t, *p_mqtt_recv_msg_t, mqtt_send_msg_t, *p_mqtt_send_msg_t;


//#define     BYTE0(temp)                        *((uint8_t *)(&temp))
//#define     BYTE1(temp)                        *((uint8_t *)(&temp)+1)
//#define     BYTE2(temp)                        *((uint8_t *)(&temp)+2)
//#define     BYTE3(temp)                        *((uint8_t *)(&temp)+3)
///*
//*   烟雾传感器
////  "ProductKey": "a1HflwuvkIR",
////  "DeviceName": "iot_mq2",
////  "DeviceSecret": "eb59bb2d088a36d1fe44af6a9fd301d9"
//*/
//extern u8  mqtt_txbuf[256];
//extern u16 mqtt_txlen;


//extern uint8_t MQTT_Connect_Pack(u8 *mqtt_buff,u32 *mqtt_len);
//extern void MQTT_SendHeart(void);
//extern u8 MQTT_PublishData_Pack(char *topic ,char *message, u8 qos,u8 *mqtt_txbuf,u32 *len);
//extern uint8_t MQTT_SubsrcibeTopic(char *topic,uint8_t qos,uint8_t whether,u8 *mqtt_txbuf,u32 *len);

uint8_t MQTT_Connect(void);
int32_t MQTT_PingReq(int32_t sock);
int32_t MQTTSubscribe(int32_t sock,char *topic,enum QoS pos);
void UserMsgCtl(MQTT_USER_MSG  *msg);
uint16_t GetNextPackID(void);
int32_t ReadPacketTimeout(int32_t sock,uint8_t *buf,int32_t buflen,uint32_t timeout);
int32_t WaitForPacket(int32_t sock,uint8_t packettype,uint8_t times);
extern void Sky_mqtt_thread_init(void);
#endif