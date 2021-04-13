#ifndef __LED_H
#define __LED_H
#include "stm32f4xx_hal.h"

#define LED0_Pin            GPIO_PIN_1
#define LED1_Pin            GPIO_PIN_0
#define LED_Port            GPIOB


extern void led_init(void);

#endif