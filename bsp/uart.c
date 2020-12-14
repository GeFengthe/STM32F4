#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

u8 *rx_data;
void init_uart1(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    USART_InitTypeDef   UART_InitStruct;
    NVIC_InitTypeDef    NVIC_InitStruct;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    
    GPIO_PinAFConfig(UART1_Port,UART1_TX_SOURCE,GPIO_AF_USART1);
    GPIO_PinAFConfig(UART1_Port,UART1_RX_SOURCE,GPIO_AF_USART1);
    
    GPIO_InitStruct.GPIO_Pin = UART1_TX_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(UART1_Port,&GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = UART1_RX_Pin;
    GPIO_Init(UART1_Port,&GPIO_InitStruct);
    
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    
    UART_InitStruct.USART_BaudRate = 115200;
    UART_InitStruct.USART_Parity = USART_Parity_No;
    UART_InitStruct.USART_WordLength = USART_WordLength_8b;
    UART_InitStruct.USART_StopBits = USART_StopBits_1;
    UART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1,&UART_InitStruct);
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART1,ENABLE);
    
}

void SendByte(USART_TypeDef * pUSARTx,uint8_t ch)
{
    USART_SendData(pUSARTx,ch);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}
int fputc(int ch,FILE *f)
{
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
    USART_SendData(USART1,(unsigned char) ch);
    return ch;
    
}

void SendData(u8 len, u8 *byte )
{
    u8 i = 0;
    for(;i<len;i++)
    {
        SendByte(USART1,byte[i]);
    }
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET)
    {
        
    }
}

void USART1_IRQHandler(void)
{

    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
    {
        rx_data[0]=USART_ReceiveData(USART1);
        SendByte(USART1,rx_data[0]);
    }
}
