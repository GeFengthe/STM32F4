#include "transport.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "string.h"

static int mysock;

/***
*@beif  transport_sendPackBuffer
*@para  uint8* uint32_t 
*@data  2021-5-28
*@auth  GECHENG
*@note  以TCP方式发送数据
*/
int32_t transport_sendPacketBuffer(uint8_t* buf,uint32_t buflen)
{
    int32_t rc;
    rc =write(mysock, buf, buflen);
    return rc;
}

/***
*@beif  transport_sendPackBuffer
*@para  uint8* uint32_t 
*@data  2021-5-28
*@auth  GECHENG
*@note  接收TCP数据
*/

int32_t transport_getdata(uint8_t* buf, int32_t count)
{
    int32_t rc;
    
    rc =recv(mysock, buf, count, 0);
    return rc;
}

/***
*@beif  transport_sendPackBuffer
*@para  uint8* uint32_t 
*@data  2021-5-28
*@auth  GECHENG
*@note  打开一个接口，并且和服务器建立连接
*/
int32_t transport_open(int8_t* servip, int32_t port)
{
    int32_t *sock =&mysock;
    int32_t ret;
    struct sockaddr_in addr;
    
    memset(&addr,0,sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family =AF_INET;
    //填写服务器端口号
    addr.sin_port = PP_HTONS(port);
    //填写服务器IP地址
    addr.sin_addr.s_addr = inet_addr((const char*)servip);
    
    //创建sock
    *sock =socket(AF_INET,SOCK_STREAM,0);
    //连接服务器
    ret =connect(*sock,(struct sockaddr*)&addr,sizeof(addr));
    if(ret !=0)
    {
        //关闭连接
        close(*sock);
        return -1;
    }
    return *sock;
}
/***
*@beif  transport_sendPackBuffer
*@para  uint8* uint32_t 
*@data  2021-5-28
*@auth  GECHENG
*@note  关闭套接字
*/
int transport_close(void)
{
    int rc;
    rc = shutdown(mysock, SHUT_WR);
    rc = recv(mysock, NULL, (size_t)0,0);
    rc = close(mysock);
    return rc;
}