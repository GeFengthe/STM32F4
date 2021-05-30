#include "udp_demo.h"
#include "lwip_comm.h"
#include "lwip/api.h"
#include "string.h"

//udp客户端任务
#define UDP_PRIO            10
//任务堆栈大小
#define UDP_STK_SIZE        512
//任务控制块
TaskHandle_t udp_thread;

u8 udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];

const u8 *udp_demo_sendbuf="SKY world STM32F4/F7 NETCONN UDP demo send data\r\n";

u8 udp_flag;                                                //udp 数据发送标志

static void Sky_udpthread(void *parameter)
{
    static struct netconn *udpconn;                         //udp连接结构体
    static struct netbuf *recvbuf;                          //接收数据
    static struct netbuf *sentbuf;                          //发送数据
    struct ip_addr destipaddr;
    struct pbuf *q;
    u32 data_len =0;
    err_t err;
    
    udpconn =netconn_new(NETCONN_UDP);                      //创建一个udp连接
    udpconn->recv_timeout =10;                              //netconn_recv()会阻塞线程,这里设置超时时间为10ms，就不会阻塞线程，但是要使用recv_timeout 字段要在lwipopts.h中定义LWIP_SO_RECVTIMEO
    
    if(udpconn !=NULL)
    {
        err =netconn_bind(udpconn,IP_ADDR_ANY,UDP_DEMO_PORT);           //将udpconn连接结构与本地IP地址和端口号绑定
        IP4_ADDR(&destipaddr,lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);      //构造目的IP的地址
        netconn_connect(udpconn,&destipaddr,UDP_DEMO_PORT);                     //连接远端主机
        
        if(err ==ERR_OK)
        {
            while(1)
            {
                if((udp_flag &LWIP_SEND_DATA)==LWIP_SEND_DATA)
                {
                    
                    sentbuf =netbuf_new();
                    netbuf_alloc(sentbuf,strlen((char *)udp_demo_sendbuf));
                    MEMCPY(sentbuf->p->payload,(void *)udp_demo_sendbuf,strlen((char *)udp_demo_sendbuf)); 
                    err =netconn_send(udpconn,sentbuf);
                    
                    if(err !=ERR_OK)
                    {
                        printf("发送失败\r\n");
                        netbuf_delete(sentbuf);
                    }
                    udp_flag &= ~LWIP_SEND_DATA;
                    netbuf_delete(sentbuf);
                }
                
                netconn_recv(udpconn,&recvbuf);                     //接收数据
                
                if(recvbuf !=NULL)
                {
                    taskENTER_CRITICAL();                           //进入临界段，关闭中断
                    memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);
                    
                    for(q =recvbuf->p;q!=NULL;q =q->next)
                    {
                        if(q->len >(UDP_DEMO_RX_BUFSIZE -data_len))
                        {
                            memcpy(udp_demo_recvbuf+data_len,q->payload,(UDP_DEMO_RX_BUFSIZE-data_len));                //拷贝数据
                        }else{
                            memcpy(udp_demo_recvbuf+data_len,q->payload,q->len);
                        }
                        data_len +=q->len;
                        if(data_len >UDP_DEMO_RX_BUFSIZE)
                            break;     
                    }
                    taskEXIT_CRITICAL();
                    data_len =0;
                    printf("%s \r\n",udp_demo_recvbuf);
                    netbuf_delete(recvbuf);
                    
                }
                vTaskDelay(5);              //延迟5ms
            }
        }else{
            printf("UDP 绑定失败\r\n");
        }
        
    }else{
        printf("UDP 连接创建失败\r\n");
    }
}


/*创建UDP线程*/
void udp_demo_init(void)
{
    UBaseType_t res;
    res =xTaskCreate((TaskFunction_t )Sky_udpthread,
                     (const char *) "udpdemo",
                     (uint16_t )    UDP_STK_SIZE,
                     (void *) NULL,
                     (UBaseType_t ) UDP_PRIO,
                     (TaskHandle_t *)&udp_thread);
    if(res ==NULL)
    {
        printf("ude thread fail\r\n");
    }
//    vTaskStartScheduler();    
}