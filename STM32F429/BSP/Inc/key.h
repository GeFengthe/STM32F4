#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx_hal.h"

#define KEY_UP_PIN                          GPIO_PIN_0
#define KEY_UP_PORT                         GPIOA
#define KEY2_PIN                            GPIO_PIN_13
#define KEY2_PORT                           GPIOC

#define KEY1_PIN                            GPIO_PIN_2
#define KEY1_PORT                           GPIOH
#define KEY0_PIN                            GPIO_PIN_3
#define KEY0_PORT                           GPIOH


enum{
    KEY_INIT_MODE =0,
    KEY_UP_DOWN =1,
    KEY0_DOWN,
    KEY1_DOWN,
    KEY2_DOWN
};

extern uint8_t sky_keymode;

extern void key_init(void);
extern void Tim3_init(uint16_t arr,uint16_t psc);
#endif