#ifndef __KEY_H
#define __KEY_H
#include "main.h"

#define KEY0_PIN        GPIO_PIN_3
#define KEY0_Port       GPIOH

#define KEY1_PIN        GPIO_PIN_2
#define KEY1_Port       GPIOH

#define KEY2_PIN        GPIO_PIN_13
#define KEY2_Port       GPIOC

#define KEYUP_PIN       GPIO_PIN_0
#define KEYUP_Port      GPIOA


#define KEY0            HAL_GPIO_ReadPin(KEY0_Port,KEY0_PIN)
#define KEY1            HAL_GPIO_ReadPin(KEY1_Port,KEY1_PIN)
#define KEY2            HAL_GPIO_ReadPin(KEY2_Port,KEY2_PIN)
#define KEYUP           HAL_GPIO_ReadPin(KEYUP_Port,KEYUP_PIN)


#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

void key_init(void);

uint8_t Sky_KeyScan(uint8_t mode);

#endif
