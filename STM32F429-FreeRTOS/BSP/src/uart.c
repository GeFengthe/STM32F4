#include "uart.h"
#include "stdio.h"

UART_HandleTypeDef  UART1_Handler;              //UART1句柄

void uart1_init(void)
{
    UART1_Handler.Instance =USART1;
    UART1_Handler.Init.BaudRate =115200;
    UART1_Handler.Init.WordLength =USART_WORDLENGTH_8B;
    UART1_Handler.Init.StopBits =USART_STOPBITS_1;
    UART1_Handler.Init.Parity =USART_PARITY_NONE;
    UART1_Handler.Init.Mode = USART_MODE_TX_RX;
    UART1_Handler.Init.HwFlowCtl =UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART1_Handler);
    __HAL_UART_ENABLE_IT(&UART1_Handler,USART_IT_RXNE);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    if(huart->Instance ==USART1)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();
        
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode =GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull =GPIO_PULLUP;
        GPIO_InitStruct.Speed =GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate =GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
        
        GPIO_InitStruct.Pin =GPIO_PIN_10;
        HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
        
        HAL_NVIC_EnableIRQ(USART1_IRQn);            //使能USART1_IROn中断通道
        HAL_NVIC_SetPriority(USART1_IRQn,2,0);
    }    
}

/**
 * @brief	重定义fputc函数
 *
 * @param	ch		输出字符量
 * @param	f		文件指针
 *
 * @return  void
 */
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕

    USART1->DR = (uint8_t) ch;
    return ch;
}