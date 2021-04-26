#ifndef __LED_H
#define __LED_H
#include "stm32f4xx_hal.h"

#define LEDR_Pin            GPIO_PIN_1
#define LEDG_Pin            GPIO_PIN_0
#define LED_Port            GPIOB

#define LEDR(n)             n?HAL_GPIO_WritePin(LED_Port,LEDR_Pin,GPIO_PIN_RESET):HAL_GPIO_WritePin(LED_Port,LEDR_Pin,GPIO_PIN_SET)
#define LEDG(n)             n?HAL_GPIO_WritePin(LED_Port,LEDG_Pin,GPIO_PIN_RESET):HAL_GPIO_WritePin(LED_Port,LEDG_Pin,GPIO_PIN_SET)

extern void led_init(void);

#endif