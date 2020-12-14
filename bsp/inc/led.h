#ifndef __LED_H
#define __LED_H
#include "stm32f4xx.h"

#define RED_GPIO_Pin        GPIO_Pin_10
#define RED_GPIO_Port       GPIOH
#define GREEN_GPIO_Pin      GPIO_Pin_11
#define GREEN_GPIO_Port     GPIOH
#define BULE_GPIO_Pin       GPIO_Pin_12
#define BULE_GPIO_Port      GPIOH


void init_led(void);
void LedSwitch(u32 Pin, u8 mode);
#endif