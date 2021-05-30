#include "key.h"

void key_init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    
    GPIO_InitStruct.Pin =KEY2_PIN;
    GPIO_InitStruct.Mode =GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull =GPIO_PULLUP;
    GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY2_Port,&GPIO_InitStruct);
    
    GPIO_InitStruct.Pin =KEY0_PIN|KEY1_PIN;
    HAL_GPIO_Init(KEY0_Port,&GPIO_InitStruct);
    
    GPIO_InitStruct.Pin =KEYUP_PIN;
    GPIO_InitStruct.Pull =GPIO_PULLDOWN;
    GPIO_InitStruct.Mode =GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed =GPIO_SPEED_FAST;
    HAL_GPIO_Init(KEYUP_Port,&GPIO_InitStruct);
}

uint8_t Sky_KeyScan(uint8_t mode)
{
    
        static uint8_t key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEYUP==1))
    {
        delay_ms(10);
        key_up=0;
        if(KEY0==0)       return 1;
        else if(KEY1==0)  return 2;
        else if(KEY2==0)  return 3;
        else if(KEYUP==1) return 4;          
    }else if(KEY0==1&&KEY1==1&&KEY2==1&&KEYUP==0)key_up=1;
    return 0;   //无按键按下
}


