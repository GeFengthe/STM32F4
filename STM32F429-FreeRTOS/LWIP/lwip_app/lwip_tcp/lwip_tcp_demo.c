#include "lwip_tcp_demo.h"
#include "lwip/api.h"
#include "string.h"
#include "mqtt.h"
#include "delay.h"

#define MQTT_CONNECT_ACK                4
#define MQTT_DISCONNECT_ACK             2
#define MQTT_HEART_ACK                  2
#define MQTT_SUBTOPIC_ACK               2

typedef enum{
    MQTT_CONNECT_START=1,
    MQTT_CONNECT_SUCCESS,
    MQTT_PUBLISHDATA,
    MQTT_SUB_TOPIC,    
}MQTT_CONNECT_STATE;

struct netconn *tcp_clientconn;                 //TCP_CLIENT连接结构体



u8 tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];                         //TCP客户端接收数据缓冲区
u8 *tcp_client_sendbuf;                         //TCP客户端发送数据缓冲区
u32 tcp_len;
u8 mqtt_alive;                                  //是否连接上MQTT标志
u8 subtopic_flag;                    


//连接成功服务器回应 20 02 00 00
//客户端主动断开连接 e0 00

const u8 parket_connetAck[] = {0x20,0x02,0x00,0x00};
const u8 parket_disconnect[] = {0xe0,0x00};
const u8 parket_heart[] = {0xc0,0x00};
const u8 parket_heart_reply[] ={0xc0,0x00};
const u8 parket_subAck[] ={0x90,0x03};

//TCP客户端任务优先级
#define SKY_TCPCLIENT_PRIO                  11

//任务堆栈大小
#define SKY_TCPCLIENT_STK_SIZE              512

//TCP客户端任务句柄
TaskHandle_t Sky_TcpClientThread_Handler;             //TCP客户端线程


/**
*@beif 
*/

void mqtt_sendheart(void)
{
    lwip_tcpClient_send((u8 *)parket_heart,2);
}


void lwip_tcpClient_send(u8 * sendbuf,u32 length)
{
    err_t err;
    err=netconn_write(tcp_clientconn,sendbuf,length,NETCONN_COPY);
    if(err !=ERR_OK)
    {
      printf("发送失败\r\n");
    }
}
/***
*@beif  mqtt_MessageProcess
*@para  mag,length,type
*@data  2021-5-28
*@auth  GECHENG
*/
static void mqtt_MessageProcess(u8 *mag,u32 length)
{
    if(length ==MQTT_CONNECT_ACK)
    {
        if(mag[0] ==parket_connetAck[0]&& mag[1] ==parket_connetAck[1]&&mag[2] ==parket_connetAck[3])
        {
            mqtt_alive =1;                                                                              //MQTT阿里云在线标志
        }
    }else if(length ==MQTT_DISCONNECT_ACK)
    {
        if(mag[0] ==parket_disconnect[0]&&mag[1] ==parket_disconnect[1])
        {
            mqtt_alive =0;
        }
    }else if(length ==MQTT_SUBTOPIC_ACK)
    {
        if(mag[0] ==parket_subAck[0]&&mag[1] ==parket_subAck[1])
        {
            subtopic_flag =1;                                                                           //主题订阅成功标志
        }
    }
}

/***
*@beif  Sky_TcpClientThread
*@para  void
*@data  2021-5-30
*@auth  GECHENG
*@note  MQTT客户端线程，优先级为10
*/
static void Sky_TcpClientThread(void *arg)
{
    u32 data_len =0;
    struct pbuf *q;
    static ip_addr_t server_ipaddr,loca_ipaddr;
    static u16_t server_port,loca_port;
    err_t err,recv_err;
    server_port=REMOTE_PORT;                                                                                                        //远端端口号
    IP4_ADDR(&server_ipaddr,lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);                       //将初始化结构体lwipdev的远端主机IP地址转换
    mqtt_alive =0;                                                                                      //初始化MQTT客户端
    tcp_clientconn =netconn_new(NETCONN_TCP);                                                           //创建一个TCP连接
    err =netconn_connect(tcp_clientconn,&server_ipaddr,server_port);                                    //连接服务器    
    if(err !=ERR_OK)
    {
        netconn_delete(tcp_clientconn);
        printf("连接服务器失败\r\n");
    }else if(err==ERR_OK)
    {
        struct netbuf *recvbuf;                                                                         //接收缓冲区
        tcp_clientconn->recv_timeout =10;    
        netconn_getaddr(tcp_clientconn,&loca_ipaddr,&loca_port,1);
        printf("连接上服务器%d.%d.%d.%d,本地端口号为:%d\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3],loca_port);
        tcp_client_sendbuf =pvPortMalloc(200);
        
        MQTT_Connect_Pack(tcp_client_sendbuf,&tcp_len);
        printf("tcp_len =%d\r\n",tcp_len);
        lwip_tcpClient_send(tcp_client_sendbuf,tcp_len);
        
        vPortFree(tcp_client_sendbuf);
        printf("err=%d\r\n",err);
        while(1)
        {       
            if((recv_err =netconn_recv(tcp_clientconn,&recvbuf))==ERR_OK)
            {
                taskENTER_CRITICAL();
                    
                memset(tcp_client_recvbuf,0,TCP_CLIENT_RX_BUFSIZE);
                for(q=recvbuf->p;q!=NULL;q=q->next)                                                                         //接收TCP服务端数据
                {
                    if(q->len >(TCP_CLIENT_RX_BUFSIZE-data_len))
                    {
                        memcpy(tcp_client_recvbuf+data_len,q->payload,(TCP_CLIENT_RX_BUFSIZE-data_len));
                            
                    }else
                    {
                        memcpy(tcp_client_recvbuf+data_len,q->payload,q->len);
                    }
                        data_len+=q->len;
                        if(data_len>TCP_CLIENT_RX_BUFSIZE)
                        {
                            data_len =TCP_CLIENT_RX_BUFSIZE;
                            break;
                        }
                }
                mqtt_MessageProcess(tcp_client_recvbuf,data_len);
                data_len =0;
                taskEXIT_CRITICAL();
                
                printf("%s\r\n",tcp_client_recvbuf);
                netbuf_delete(recvbuf);
             }else if(recv_err ==ERR_CLSD)
             {
                 netconn_close(tcp_clientconn);
                 netconn_delete(tcp_clientconn);
                 printf("服务器%d.%d.%d.%d断开连接\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);
                 break;
             }else if(recv_err ==ERR_TIMEOUT)
             {
                 printf(" tcp task TIMEOUT\r\n");
             }
                vTaskDelay(10);
         }
            
    }
        
}


/***
*@beif  Sky_TcpClientThread_Init
*@para  void
*@data  2021-5-30
*@auth  GECHENG
*@note  TCP作为MQTT客户端线程初始化，优先级为10
*/
void Sky_TcpClientThread_Init(void)
{
    BaseType_t err;
    err =xTaskCreate((TaskFunction_t )Sky_TcpClientThread,
                     (const char *)"tcp_cient",
                     (uint32_t ) SKY_TCPCLIENT_STK_SIZE,
                     (void *) NULL,
                     (UBaseType_t ) SKY_TCPCLIENT_PRIO,
                     (TaskHandle_t *)&Sky_TcpClientThread_Handler);
    if(err !=pdPASS)
    {
        printf("Tcp Client Thread Fail\r\n");
    }
}

