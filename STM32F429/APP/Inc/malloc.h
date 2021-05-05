#ifndef __MALLOC_H
#define __MALLOC_H
#include "stm32f4xx.h"

//定义三个内存池
#define  SRAMIN                                 0                               //内部内存池
#define  SRAMEX                                 1                               //外部内存池
#define  SRAMCCM                                2                               //CCM内存池（此部分仅CPU可以访问)

#define  SRAMBANK                               3                               //定义支持的SRAM数


#define  MEM1_BLOCK_SIZE                        64                           //内存块大小为64字节
#define  MEM1_MAX_SIZE                          100*1024                    //最大管理0x2000000 -0x20020000  128k ,0x20020000后面还有64k ，0x1000 0000映射的64k 只能cpu访问
#define  MEM1_ALLOC_TABLE_SIZE                  MEM1_MAX_SIZE/MEM1_BLOCK_SIZE  //内存表大小 2048

//内存管理控制器
struct _m_mallco_dev
{
    void (*init) (void);                                         //初始化
    uint16_t (*perused)(void);                                  //内存使用率
    uint8_t *membase;                                               //内存池 管理SRAMBANK个区域的内存
    uint32_t *memmap;                                             //内存管理状态表
    uint8_t memrdy;                                                 //内存管理是否就绪
};




void mymemset(void *s,uint8_t c,uint32_t count);	                 //设置内存
void mymemcpy(void *des,void *src,uint32_t n);                      //复制内存     
void my_mem_init(void);				                         //内存管理初始化函数(外/内部调用)
uint32_t my_mem_malloc(uint32_t size);	                //内存分配(内部调用)
uint8_t my_mem_free(uint32_t offset);		            //内存释放(内部调用)
uint16_t my_mem_perused(void) ;			                    //获得内存使用率(外/内部调用) 


void myfree(void *ptr);
void *mymalloc(uint32_t size);

#endif

