#ifndef __LAN8720_H
#define __LAN8720_H
#include "stm32f4xx.h"
#include "malloc.h"

extern ETH_HandleTypeDef ETH_Handler;               //以太网句柄
extern ETH_DMADescTypeDef *DMARxDscrTab;			//以太网DMA接收描述符数据结构体指针
extern ETH_DMADescTypeDef *DMATxDscrTab;			//以太网DMA发送描述符数据结构体指针 
extern uint8_t *Rx_Buff; 							//以太网底层驱动接收buffers指针 
extern uint8_t *Tx_Buff; 							//以太网底层驱动发送buffers指针
extern ETH_DMADescTypeDef  *DMATxDescToSet;			//DMA发送描述符追踪指针
extern ETH_DMADescTypeDef  *DMARxDescToGet; 		//DMA接收描述符追踪指针 

uint8_t LAN8720_Init(void);
uint32_t LAN8720_ReadPHY(uint16_t reg);
void LAN8720_WritePHY(uint16_t reg,uint16_t value);
uint8_t LAN8720_Get_Speed(void);
uint8_t ETH_MACDMA_Config(void);
uint8_t ETH_Mem_Malloc(void);

uint32_t ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDscr);
void ETH_Mem_Free(void);
#endif

