#include "cJSON_Process.h"
#include "FreeRTOS.h"

/*******************************************************************
 *                          变量声明                               
 *******************************************************************/

cJSON *cJSON_Init(void)
{
    cJSON* cJSON_Root = NULL;                       //json根节点
    
    cJSON_Root =cJSON_CreateObject();               //创建项目
    if(NULL ==cJSON_Root)
    {
        return NULL;
    }
    cJSON_AddStringToObject(cJSON_Root, NAME, DEFAULT_NAME);        //添加元素 键值
    cJSON_AddNumberToObject(cJSON_Root, LIGHT_STU, DEFAULT_STU);
    
    char *p =cJSON_Print(cJSON_Root);
    
    vPortFree(p);
    p=NULL;
    return cJSON_Root;
}

uint8_t cJSON_Update(const cJSON * const object,const char * const string,void *d)
{
  cJSON* node = NULL;    //json根节点
  node = cJSON_GetObjectItem(object,string);
  if(node == NULL)
    return NULL;
  if(cJSON_IsBool(node))
  {
    int *b = (int*)d;
//    printf ("d = %d",*b);
    cJSON_GetObjectItem(object,string)->type = *b ? cJSON_True : cJSON_False;
//    char* p = cJSON_Print(object);    /*p 指向的字符串是json格式的*/
    return 1;
  }
  else if(cJSON_IsString(node))
  {
    cJSON_GetObjectItem(object,string)->valuestring = (char*)d;
//    char* p = cJSON_Print(object);    /*p 指向的字符串是json格式的*/
    return 1;
  }
  else if(cJSON_IsNumber(node))
  {
    double *num = (double*)d;
//    printf ("num = %f",*num);
//    cJSON_GetObjectItem(object,string)->valueint = (double)*num;
    cJSON_GetObjectItem(object,string)->valuedouble = (double)*num;
//    char* p = cJSON_Print(object);    /*p 指向的字符串是json格式的*/
    return 1;
  }
  else
    return 1;
}

void Proscess(void *data)
{
    printf("开始解析JSON数据");
    cJSON *root, *json_name, *json_stu;
    root =cJSON_Parse((char *)data);
    
    json_name =cJSON_GetObjectItem(root, NAME);
    json_stu =cJSON_GetObjectItem(root, LIGHT_STU);
}

