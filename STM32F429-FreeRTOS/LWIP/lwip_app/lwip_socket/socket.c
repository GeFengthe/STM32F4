#include "main.h"
#include "socket.h"
#include "lwip/opt.h"
//#include "lwip/sys.h"
#include "lwip/api.h"

#include "lwip/sockets.h"
#include "string.h"


#define PORT        5001
#define IP_ADDR     "192.168.3.44"

static void client_thread(void *thread_param)
{
    int sock =-1;
    struct sockaddr_in client_addr;
    
    uint8_t send_buf[]="this si a test client..\n";
    
    while(1)
    {
        sock =socket(AF_INET,SOCK_STREAM,0);
        
        if(sock <0)
        {
            printf("socket error\n");
            vTaskDelay(10);
            continue;
        }
        
        client_addr.sin_family =AF_INET;
        client_addr.sin_port =htons(PORT);
        client_addr.sin_addr.s_addr =inet_addr(IP_ADDR);
        memset(&(client_addr.sin_zero),0,sizeof(client_addr.sin_zero));
        
        if(connect(sock,(struct sockaddr *)&client_addr,sizeof(struct sockaddr))==-1)
        {
            printf("connect failed\n");
            closesocket(sock);
            vTaskDelay(10);
            continue;
        }
        printf("connet ip server sucessful\n");
        
        while(1)
        {
            if(write(sock,send_buf,sizeof(send_buf))<0)
            {
                break;
            }
            vTaskDelay(1000);
        }
        closesocket(sock);
    }
}

void client_init(void)
{
    sys_thread_new("client",client_thread,NULL,512,11 );
}