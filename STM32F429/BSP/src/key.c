#include "key.h"
#include "stdio.h"
TIM_HandleTypeDef   TIM3_handler;
uint8_t sky_keymode;
void key_init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    
    GPIO_InitStruct.Pin =KEY_UP_PIN;
    GPIO_InitStruct.Mode =GPIO_MODE_INPUT;
    HAL_GPIO_Init(KEY_UP_PORT,&GPIO_InitStruct);
    
    GPIO_InitStruct.Pin =KEY0_PIN;
    GPIO_InitStruct.Mode =GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull =GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_PORT,&GPIO_InitStruct);
    
    GPIO_InitStruct.Pin =KEY1_PIN;
    HAL_GPIO_Init(KEY1_PORT,&GPIO_InitStruct);

    GPIO_InitStruct.Pin =KEY2_PIN;
    HAL_GPIO_Init(KEY2_PORT,&GPIO_InitStruct);
}

void Tim3_init(uint16_t arr,uint16_t psc)
{
    __HAL_RCC_TIM3_CLK_ENABLE();
    TIM3_handler.Instance =TIM3;
    TIM3_handler.Init.Period =arr;
    TIM3_handler.Init.Prescaler =psc;
    TIM3_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    TIM3_handler.Init.CounterMode =TIM_COUNTERMODE_UP;
    TIM3_handler.Init.AutoReloadPreload =TIM_AUTOMATICOUTPUT_ENABLE;   
    HAL_TIM_Base_Init(&TIM3_handler);
    
    HAL_NVIC_SetPriority(TIM3_IRQn,3,0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    HAL_TIM_Base_Start_IT(&TIM3_handler);
}

static void Sky_KeyupScan(void)
{
    static uint16_t count =0;
    if(HAL_GPIO_ReadPin(KEY_UP_PORT,KEY_UP_PIN) ==1)
    {
        count++;
    }else{
        if(count >=2)
        {
            sky_keymode =KEY_UP_DOWN;
            count =0;
        }
    }
}

static void Sky_Key0Scan(void)
{
    static uint16_t count =0;
    if(HAL_GPIO_ReadPin(KEY0_PORT,KEY0_PIN) ==0)
    {
        count++;
    }else{
        if(count >=2)
        {
            sky_keymode =KEY0_DOWN;
            count =0;
        }
    }
}

static void Sky_Key1Scan(void)
{
    static uint16_t count =0;
    if(HAL_GPIO_ReadPin(KEY1_PORT,KEY1_PIN) ==1)
    {
        count++;
    }else{
        if(count >=2)
        {
            sky_keymode =KEY1_DOWN;
            count =0;
        }
    }
}

static void Sky_Key2Scan(void)
{
    static uint16_t count =0;
    if(HAL_GPIO_ReadPin(KEY2_PORT,KEY2_PIN) ==1)
    {
        count++;
    }else{
        if(count >=2)
        {
            sky_keymode =KEY2_DOWN;
            count =0;
        }
    }
}


void TIM3_IRQHandler(void)
{
    if(__HAL_TIM_GET_ITSTATUS(&TIM3_handler,TIM_IT_UPDATE) !=RESET)
    {
        Sky_KeyupScan();
        Sky_Key0Scan();
        Sky_Key1Scan();
        Sky_Key2Scan();
//        printf("tim3\r\n");
        __HAL_TIM_CLEAR_IT(&TIM3_handler,TIM_IT_UPDATE);
    }
}


