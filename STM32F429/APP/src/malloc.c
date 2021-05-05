#include "malloc.h"

//内存池(32字节对齐)
//__align(32) uint8_t mem1base[MEM1_MAX_SIZE] __attribute__ ((at(0x20000000)));                                                  //内部SRAM内存池

__align(32) uint8_t mem1base[MEM1_MAX_SIZE];
//内存管理表
uint32_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];

//内存管理参数
// const uint32_t memtblsize[SRAMBANK];

//内存管理控制器
struct  _m_mallco_dev mallco_dev =
{
    my_mem_init,
    my_mem_perused,
    mem1base,
    mem1mapbase,
    0
};
/**
 * 设置内存
 * *s:内存首地址
 * c:要设置的值
*/
void mymemset(void *s,uint8_t c,uint32_t count)
{
    uint8_t *xs = s;
    while(count--)
    {
        *xs++ =c;
    }
}

/**
 * 内存管理初始化
 * memx：所属内存块
*/
void my_mem_init(void)
{
    mymemset(mallco_dev.memmap,0,MEM1_ALLOC_TABLE_SIZE *4);
    mallco_dev.memrdy =1;
}
/**
 * 获取内存使用率
 * 
*/
uint16_t my_mem_perused(void)
{
    uint32_t used =0;
    uint32_t i;
    for(i =0;i<MEM1_ALLOC_TABLE_SIZE;i++)
    {
        if(mallco_dev.memmap[i])
        {
            used++;
        }
    }
    return (used*1000)/(MEM1_ALLOC_TABLE_SIZE);
}

/**
 * 内存分配（内部调用）
 * 
*/
uint32_t my_mem_malloc(uint32_t size)
{
    signed long offset =0;
    uint32_t nmemb;                                                 //需要的内存块数
    uint32_t cmemb =0;                                              //连续内存块数
    uint32_t i;
    if(!mallco_dev.memrdy)
    {
        mallco_dev.init();
    }
    if(size ==0)
    {
        return 0xFFFFFFFF;                                          //不需要分配
    }
    nmemb =size/MEM1_BLOCK_SIZE;
    if(size %MEM1_BLOCK_SIZE)
    {
        nmemb++;
    }
    for(offset =MEM1_ALLOC_TABLE_SIZE-1;offset >=0;offset--)
    {
        if(!mallco_dev.memmap[offset])
        {
            cmemb++;
        }else
        {
            cmemb =0;
        }
        if(cmemb ==nmemb)
        {
            for(i =0;i<nmemb;i++)
            {
                mallco_dev.memmap[offset+i] =nmemb;
            }
            return (offset*MEM1_BLOCK_SIZE);
        }
    }
    return 0xFFFFFFFF;
}

/**
 * 释放内存
*/
uint8_t my_mem_free(uint32_t offset)
{
    int i;
    if(!mallco_dev.memrdy)
    {
        mallco_dev.init();
        return 1;                                   //未初始化
    }
    if(offset <MEM1_MAX_SIZE)
    {
         int index =offset/MEM1_BLOCK_SIZE;
         int nmemb =mallco_dev.memmap[index];
         for(i =0;i<nmemb;i++)
         {
             mallco_dev.memmap[index+i]=0;
         }
         return 0;
    }else return 2;
}
/**
 * 释放内存(外部调用)
 * 
*/
void myfree(void *ptr)
{
    uint32_t offset;
    if(ptr ==NULL)
    return;
    offset =(uint32_t)ptr -(uint32_t)mallco_dev.membase;
    my_mem_free(offset);
}

/**
 * 分配内存(外部调用)
*/
void *mymalloc(uint32_t size)
{
    uint32_t offset;
    offset =my_mem_malloc(size);
    if(offset ==0xFFFFFFFF)
    {
        return NULL;
    }else{
        return (void*)((uint32_t)mallco_dev.membase+offset);
    }
}