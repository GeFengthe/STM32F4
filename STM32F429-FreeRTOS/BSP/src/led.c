#include "led.h"
void led_init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
   __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStruct.Pin =LEDR_Pin;
    GPIO_InitStruct.Mode =GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull =GPIO_PULLUP;
    GPIO_InitStruct.Speed =GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED_Port,&GPIO_InitStruct);
    
    GPIO_InitStruct.Pin =LEDG_Pin;
    HAL_GPIO_Init(LED_Port,&GPIO_InitStruct);
}

