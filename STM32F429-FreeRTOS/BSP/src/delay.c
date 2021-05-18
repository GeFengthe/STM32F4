#include "delay.h"

//使用FreeRTOS时用到 滴答定时器起调度xPortSysTickHandler（）
extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{  
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
    HAL_IncTick();
}

void delay_us(uint32_t nus)
{
	uint32_t tickcnt=0,ticksrt,tickend;
	uint32_t reload = SysTick->LOAD;                                         //获取装载值
    uint32_t ticktotal =nus *180;                                           //总节拍数
    ticksrt =SysTick->VAL;
    while(1)
    {
        tickend =SysTick->VAL;
        if(ticksrt >tickend)
            tickcnt +=ticksrt -tickend;
        else
            tickcnt +=reload -tickend+ticksrt;
        ticksrt =tickend;
        if(tickcnt >=ticktotal)
            break;     
    }	
}


void delay_ms(uint16_t nms)
{
    while(nms--)
    {
        delay_us(1000);
    }
}
