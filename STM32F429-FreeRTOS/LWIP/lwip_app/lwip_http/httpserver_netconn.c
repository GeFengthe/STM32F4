#include "httpserver_netconn.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "stdio.h"
#include "stdbool.h"

#define HTTP_LED 0

#if HTTP_LED
static const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";

static const char http_index_html[] = "<html><head><title>Congrats!</title></head>\
<body><h1 align=\"center\">SKY World!</h1>\
<h2 align=\"center\">Welcome to Fire lwIP HTTP Server!</h1>\
<p align=\"center\">This is a small test page, served by httpserver-netconn.</p>\
<p align=\"center\"><a href=\"http://www.firebbs.cn/forum.php/\">\
<font size=\"6\"> 葛呈电子论坛 </font> </a></p>\
<a href=\"http://www.firebbs.cn/forum.php/\">\
<p align=\"center\"><img src=\"http://nimg.ws.126.net/?url=http%3A%2F%2Fdingyue.ws.126.net%2F2021%2F0516%2F90494e09j00qt717y001fc000hs00poc.jpg&thumbnail=650x2147483647&quality=80&type=jpg\" /></a>\
</body></html>";


/***
*@beif  Sky_http_server_thread
*@para  void
*@data  2021-5-28
*@auth  GECHENG
*@note  http服务器主线程
*/

static void Sky_http_server_netconn_serve(struct netconn *conn)
{
    struct netbuf *inbuf;
    char *buf;
    uint16_t buflen;
    err_t err;
    
    //读取数据
    err =netconn_recv(conn, &inbuf);
    if(err ==ERR_OK)
    {
        netbuf_data(inbuf,(void **)&buf, &buflen);
        if(buflen >5 && buf[0] =='G' &&
                        buf[1] =='E' &&
                        buf[2] =='T' &&
                        buf[3] ==' ' &&
                        buf[4] =='/')
        {
            //发送数据头
            netconn_write(conn, http_html_hdr,sizeof(http_html_hdr)-1,NETCONN_NOCOPY);
            //发送网页数据
            netconn_write(conn,http_index_html,sizeof(http_index_html)-1,NETCONN_COPY);
        }
    }
    netconn_close(conn);
    
    netbuf_delete(inbuf);
}


#else

const static char http_html_hdr[] ="HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";

const unsigned char LED1On_Data[] ="<HTML> \
<head><title>HTTP LED Control</title></head> \
<center> \
<p> \
<font size=\"6\">LED<font style = \"color:red\"> 已打开！</font> \
<form method=post action=\"off\" name=\"ledform\"> \
<input type=\"submit\" value=\"关闭\" style=\"width:80px;height:30px;\"></form>\
</center> \
</HYML> ";

const unsigned char LED1Off_Data[] ="<HTML> \
<head><title>HTTP LED Control</title></head> \
<center> \
<p> \
<font size=\"6\">LED<font style = \"color:red\"> 已关闭！</font> \
<form method=post action=\"on\" name=\"ledform\"> \
<input type=\"submit\" value=\"打开\" style=\"width:80px;height:30px;\"></form> \
</center> \
</HTML> ";

static const char http_index_html[] ="<html><head><title>Congrats!</title></head>\
<body><h2 align=\"center\">Welcome to Fire lwIP HTTP Server!</h2>\
<p align=\"center\">This is a small test page : http control led.</p>\
<p align=\"center\"><a href=\"http://www.firebbs.cn/forum.php/\">\
<font size=\"6\"> SKY电子论坛 </font> </a></p>\
<a href=\"http://www.firebbs.cn/forum.php/\">\
<img src=\"http://www.firebbs.cn/data/attachment/portal/201806/\
05/163015rhz7mbgbt0zfujzh.jpg\"/></a>\
</body></html>";

static _Bool led_on =FALSE;

static void Sky_http_server_netconn_serve(void *arg)
{
    
}


#endif

/***
*@beif  Sky_http_server_thread
*@para  void
*@data  2021-5-28
*@auth  GECHENG
*@note  http服务器主线程
*/
static void Sky_http_server_thread(void *arg)
{
    struct netconn *conn, *newconn;
    err_t err;
    LWIP_UNUSED_ARG(arg);
    printf(" http server sucess\n");
    //创建netconn 连接结构
    //绑定端口号与IP地址，端口号默认是80
    conn =netconn_new(NETCONN_TCP);
    netconn_bind(conn,IP_ADDR_ANY,80);
//    TASK_LIST();
    LWIP_ERROR("http_server:invalid conn",(conn != NULL),return ;);
    
    //监听
    netconn_listen(conn);
    
    do
    {
        err =netconn_accept(conn, &newconn);
        if(err ==ERR_OK)
        {
            //发送网页数据
            Sky_http_server_netconn_serve(newconn);
            //删除连接结构，http不是一个长连接，客户端获取一次后，服务器就会断开这个连接
            netconn_delete(newconn);
        }
    }while(err ==ERR_OK);
    
    netconn_close(conn);
    netconn_delete(conn);
}


void Sky_http_server_thread_init(void)
{
    sys_thread_new("http_server",Sky_http_server_thread,NULL,2048,13);
}

void TASK_LIST(void )
{
    char pWriteBuffer[2048];
    for(;;)
    {
        sys_msleep(10000);
        vTaskList((char *)&pWriteBuffer);
        printf("task name   task_state  priority    stack   task_num\n");
        printf("%s\n",pWriteBuffer);
    }
    vTaskDelete(NULL);
    return ;
}