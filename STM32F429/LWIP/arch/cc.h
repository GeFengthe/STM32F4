#ifndef __CC_H__
#define __CC_H__
#include "cpu.h"
#include "stdio.h"

//定义与平台无关的数据类型
typedef unsigned    char    u8_t;
typedef signed      char    s8_t;
typedef unsigned    short   u16_t;
typedef signed      short   s16_t;
typedef unsigned    long    u32_t;
typedef signed      long    s32_t;
typedef u32_t   mem_ptr_t;                      //内存地址型数据
typedef int     sys_port_t;                     //临界保护型数据


//根据不同编译器定义一些符号
#if defined(__ICCARM__)
#define PACK_STRCUT_BEGIN


#elif defined (__CC_ARM)
#define PACK_STRUCT_BEGIN_packed
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x)x
#elif defined(__GNUC__)


#elif defined(__TASKING)


#endif
#define U16_F   "4d"
#define S16_F   "4d"
#define X16_F   "4x"

#define U32_F   "8ld"
#define S32_F   "81d"
#define X32_F   "81x"

//宏定义
#ifndef LWIP_PLATFORM_ASSERT
#define LWIP_PLATFROM_ASSERT(x)\
do\
    {   printf("Assertion\"%s\"failed at line %d in %s \r\n",x,__LINE__,__FILE__);\
    }while(0)
#endif
#ifndef LWIP_PLATFROM_DIAG
#define LWIP_PLATFROM_DLAG(x)   do{printf x;}while(0)
#endif
    
    
#endif

