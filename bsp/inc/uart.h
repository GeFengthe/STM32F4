#ifndef __UART_H
#define __UART_H
#include "stm32f4xx.h"

#define UART1_TX_Pin         GPIO_Pin_9
#define UART1_RX_Pin         GPIO_Pin_10
#define UART1_TX_SOURCE      GPIO_PinSource9
#define UART1_RX_SOURCE      GPIO_PinSource10
#define UART1_Port           GPIOA

extern u8 *rx_data;
void init_uart1(void);
void SendData(u8 len, u8 *byte );


#endif