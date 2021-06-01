#include "mqtt.h"
#include "string.h"

//#include "esp8266.h"

char MQTT_ClientID[100];                    //客户端ID
char MQTT_UserName[100];                    //用户民
char MQTT_PassWord[100];                    //秘钥


char *MQTT_CLIENTID ="00001|securemode=3,signmethod=hmacsha1|";
char *MQTT_USARNAME ="light_1&a1lHej9J2s1";
char *MQTT_PASSWORD ="C50CEEE8046D928B822C0E76808000EF6E0E3BC3";

typedef enum{
    //名字        值           报文流动方向          描述
    
    M_RESERVED1 =0  ,   //      禁止                  保留
    M_CONNECT       ,   //      客户端到服务器         客户端请求连接服务端报文
    M_CONNACK       ,   //      服务器到客户端         连接报文应答
    M_PUBLISH       ,   //      两个方向都允许          发布消息
    M_PUBACK        ,   //      两个方向都允许         QoS 1消息发布确认
    M_PUBREC        ,   //      两个方向都允许	    发布收到（保证交付第一步）
    M_PUBREL        ,   //      两个方向都允许	    发布释放（保证交付第二步）
    M_PUBCOMP       ,   //      两个方向都允许	    QoS 2消息发布完成（保证交互第三步）
    M_SUBSCRIBE     ,   //      客户端到服务端	    客户端订阅请求
    M_SUBACK        ,   //      服务端到客户端	    订阅请求报文确认
    M_UNSUBSCRIBE   ,   //      客户端到服务端	    客户端取消订阅请求
    M_UNSUBACK      ,   //      服务端到客户端	    取消订阅报文确认
    M_PINGREQ       ,   //      客户端到服务端	    心跳请求
    M_PINGRESP      ,   //      服务端到客户端	    心跳响应
    M_DISCONNECT    ,   //      客户端到服务端	    客户端断开连接
    M_RESERVED2     ,   //      禁止	                保留
    
}_typdef_mqtt_message;

/*
函数功能: 初始化阿里云物联网服务器的登录参数
*/
 
//密码
//clientId*deviceName*productKey#
// *替换为DeviceName  #替换为ProductKey  加密密钥是DeviceSecret  加密方式是HmacSHA1  
//PassWord明文=clientIdmq2_iotdeviceNamemq2_iotproductKeya1WLC5GuOfx
//hmacsha1加密网站：http://encode.chahuo.com/
//加密的密钥：DeviceSecret


//MQTT发送数据
void MQTT_SendBuf(uint8_t *buf,uint16_t len)
{
//    lwip_tcpClient_send(buf,len);
    for(uint8_t i=0;i<len;i++)
    {
        printf("buf[%d] =0x%x   ",i,buf[i]);
    }
//    ESP8266_ATSendBuf(buf,len);
}
//MQTT发送心跳
//void MQTT_SendHeart(void)
//{
//    MQTT_SendBuf((u8 *)parket_heart,sizeof(parket_heart));
//}


/*
函数功能: 登录服务器
函数返回值: 1表示成功 0表示失败
*/
uint8_t MQTT_Connect_Pack(u8 *mqtt_buff,u32 *mqtt_len)
{
    u8 encodedByte;
    int ClientIDlen =strlen(MQTT_CLIENTID);                                              //0x27    0x13          0x28
    int Usernamelen =strlen(MQTT_USARNAME);
    int Passwordlen =strlen(MQTT_PASSWORD);
    int Datalen;
    u32 buff_len=0;
//    memset(mqtt_txbuf,0,512);                                                         //指针越界访问了
    //可变报头+Payload  每个字段包含两个字节的长度标识
    Datalen = 10+(ClientIDlen+2)+(Usernamelen+2)+(Passwordlen+2);
    //固定报头
	//控制报文类型
    mqtt_buff[buff_len++]=0x10;
    printf("Datalen=%d\r\n",Datalen);
    do{
        encodedByte =Datalen %128;
        Datalen =Datalen/128;
        if(Datalen >0)
            encodedByte =encodedByte |128;                  //把最高为置1表示还有延续字节
        mqtt_buff[buff_len++] =encodedByte;
    }while(Datalen>0);
//    printf("mqtt_txbuf[1]=0x%x\r\n",mqtt_txbuf[1]);
//    printf("mqtt_txlen=%d\r\n",mqtt_txlen);
    //可变报头  (包含四个字段 协议名(protocol name) 协议级别(protocol level) 连接标志(Connect Flags) 保持连接(keep alive)
    //协议名
    mqtt_buff[buff_len++] =0;
    mqtt_buff[buff_len++] =4;
    mqtt_buff[buff_len++] ='M';
    mqtt_buff[buff_len++] ='Q';
    mqtt_buff[buff_len++] ='T';
    mqtt_buff[buff_len++] ='T';
    
    //协议级别 对于3.1.1版本 的协议级别是的值是4(0x04);
    mqtt_buff[buff_len++] =0x04;
    
    //连接标志
    mqtt_buff[buff_len++] =0xc2;
    //保持连接 keep alive 是一个以秒为单位的时间间隔，表示为一个16位的字 ，它是指在客户端发送
    mqtt_buff[buff_len++] =0x00;
    mqtt_buff[buff_len++] =60;         //60s一个心跳包
    
    //客户端ID有效载荷字段
    mqtt_buff[buff_len++] =BYTE1(ClientIDlen);                      // Client ID length MSB
    mqtt_buff[buff_len++] =BYTE0(ClientIDlen);                       // Client ID length LSB
    memcpy(&mqtt_buff[buff_len],MQTT_CLIENTID,ClientIDlen);
    buff_len +=ClientIDlen;
    printf("clientIDlen=%d\r\n",ClientIDlen);
    
    //用户名有效载荷字段
    if(Usernamelen>0)
    {
        mqtt_buff[buff_len++] =BYTE1(Usernamelen);
        mqtt_buff[buff_len++] =BYTE0(Usernamelen);
        memcpy(&mqtt_buff[buff_len],MQTT_USARNAME,Usernamelen);
        buff_len +=Usernamelen;
        printf("Usernamelen=%d\r\n",Usernamelen);
    }
    //密码有效载荷字段
    if(Passwordlen>0)
    {
        mqtt_buff[buff_len++] =BYTE1(Passwordlen);
        mqtt_buff[buff_len++] =BYTE0(Passwordlen);
        memcpy(&mqtt_buff[buff_len],MQTT_PASSWORD,Passwordlen);
        buff_len +=Passwordlen;
        printf("Passwordlen=%d\r\n",Passwordlen);
    }
    *mqtt_len =buff_len;
    return 0;   
}

//MQTT发布数据打包函数
//topic   主题 
//message 消息
//qos     消息等级 
u8 MQTT_PublishData_Pack(char *topic ,char *message, u8 qos,u8 *mqtt_txbuf,u32 *len)
{
    static u16 id=0;
    uint8_t encodedByte;
    u32 mqtt_txlen;
    int topicLength =strlen(topic);
    int messageLength =strlen(message);
    int Datalen;
    mqtt_txlen =0;
    //有效载荷的长度
    if(qos)
    {
    //数据长度   主题名          标识符 有效载荷
        Datalen =(topicLength+2)+2+messageLength;
    }else
    {
        Datalen =(topicLength+2)+messageLength;                              //qos=0的消息没有标识符
    }
    //固定报头
    mqtt_txbuf[mqtt_txlen++]=0x30;                                              //MQTT Message Type PUBLISH
    do
    {
        encodedByte=Datalen %128;
        Datalen =Datalen/128;
        if(Datalen>0)
        {
            encodedByte |=128;
        }
        mqtt_txbuf[mqtt_txlen++]=encodedByte;
    }while(Datalen >0);
    

    mqtt_txbuf[mqtt_txlen++] =BYTE1(topicLength);                           //主题长度MSB
    mqtt_txbuf[mqtt_txlen++] =BYTE0(topicLength);                           //主题长度LSB
    memcpy(&mqtt_txbuf[mqtt_txlen],topic,topicLength);
    mqtt_txlen+=topicLength;
    
    if(qos)
    {
        mqtt_txbuf[mqtt_txlen++] = BYTE1(id);
        mqtt_txbuf[mqtt_txlen++] = BYTE0(id);
        id++;
    }
    
    memcpy(&mqtt_txbuf[mqtt_txlen],message,messageLength);
    mqtt_txlen +=messageLength;
    *len =mqtt_txlen;

    return mqtt_txlen;

}
/*
函数功能: MQTT订阅/取消订阅数据打包函数
函数参数:
    topic       主题   
    qos         消息等级 0:最多分发一次  1: 至少分发一次  2: 仅分发一次
    whether     订阅/取消订阅请求包 (1表示订阅,0表示取消订阅)
返回值: 0表示成功 1表示失败
*/
uint8_t MQTT_SubsrcibeTopic(char *topic,uint8_t qos,uint8_t whether,u8 *mqtt_txbuf,u32 *len)
{
    uint8_t cnt =2;
    uint8_t wait;
    u32 mqtt_txlen;
    int topiclen =strlen(topic);                    //可变报头的长度（2字节）加上有效载荷的长度
    int Datalen =0;
    uint8_t encodedByte =0;
    mqtt_txlen =0;
    Datalen =2+(topiclen+2)+(whether?1:0);

//控制报文
//固定报头
if(whether)
{
    mqtt_txbuf[mqtt_txlen++]=0x82;                  //消息类型和标志订阅
}else
{
    mqtt_txbuf[mqtt_txlen++]=0xA2;                  //取消订阅
}
//剩余长度
do{
    encodedByte =Datalen % 128;
    Datalen =Datalen/128;
    if(Datalen >0)
    {
        encodedByte |=0x80;
    }
    mqtt_txbuf[mqtt_txlen++]=encodedByte;
}while(Datalen >0);

//报文标识符
mqtt_txbuf[mqtt_txlen++]=0x00;                          //MSB
mqtt_txbuf[mqtt_txlen++]=0x01;                          //LSB

mqtt_txbuf[mqtt_txlen++]=BYTE1(topiclen);
mqtt_txbuf[mqtt_txlen++]=BYTE0(topiclen);
memcpy(&mqtt_txbuf[mqtt_txlen],topic,topiclen);
mqtt_txlen +=topiclen;

if(whether)
{
    mqtt_txbuf[mqtt_txlen++]=qos;
}

*len =mqtt_txlen;
return 0;
}