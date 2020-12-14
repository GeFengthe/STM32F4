#ifndef __LCD_H
#define __LCD_H
#include "stm32f4xx.h"
/*部分液晶信号线的引用复用编号是AF9*/
#define GPIO_AF_LTDC_AF9                ((uint8_t)0x09)

//红色数据线
#define LTDC_R0_GPIO_PORT               GPIOH
#define LTDC_R0_GPIO_CLK                RCC_AHB1Periph_GPIOH
#define LTDC_R0_GPIO_PIN                GPIO_Pin_2
#define LTDC_R0_PINSOURCE               GPIO_PinSoucre2
#define LTDC_R0_AF                      GPIO_AF_LTDC            //使用LTDC复用编号

#define LTDC_R3_GPIO_PORT               GPIOB
#define LTDC_R3_GPIO_CLK                RCC_AHB1Periph_GPIOB
#define LTDC_R3_GPIO_PIN                GPIO_Pin_0
#define LTDC_R3_PINSOURCE               GPIO_PinSoucre0
#define LTDC_R3_AF                      GPIO_AF_LTDC_AF9            //使用LTDC复用编号

#endif