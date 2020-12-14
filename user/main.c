#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "led.h"
#include "task.h"
#include "stdio.h"
#include "uart.h"



#define APP_PRIO                1
#define LED_PRIO                2
#define BULE_PRIO               3

#define STACK_APP_SIZE          128
#define STACK_LED_SIZE          128
#define STACK_BULE_SIZE         128

TaskHandle_t * APPTaskHandle;
TaskHandle_t * LedTaskHandle;
TaskHandle_t * BuleTaskHandle;

static void APPStart(void);
static void LedON(void);
static void LedOFF(void);

int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    init_led();
    init_uart1();
    
    BaseType_t err;
    
    err=xTaskCreate( (TaskFunction_t  ) APPStart, 
                     (const char *    )    "APP",
                     (const uint16_t  )STACK_APP_SIZE,
                     (void * const    ) NULL,
                     (UBaseType_t     ) APP_PRIO,
                     (TaskHandle_t *  ) APPTaskHandle);
    if(err !=pdPASS)
    {
        LedSwitch(RED_GPIO_Pin,1);
        while(1);
    }
    vTaskStartScheduler();
    
    
}

void APPStart(void)
{
    taskENTER_CRITICAL();
    xTaskCreate ( (TaskFunction_t  ) LedON,
                  (const char *)     "LED",
                  (const uint16_t  ) STACK_LED_SIZE,
                  (void *const     ) NULL,
                  (UBaseType_t     ) LED_PRIO,
                  (TaskHandle_t *  ) LedTaskHandle);
               
                  
    xTaskCreate ( (TaskFunction_t  ) LedOFF,
                  (const char *)     "BULE",
                  (const uint16_t  ) STACK_BULE_SIZE,
                  (void *const     ) NULL,
                  (UBaseType_t     ) BULE_PRIO,
                  (TaskHandle_t *  ) BuleTaskHandle);
                  
    vTaskDelete( APPTaskHandle);
                    
    
    taskEXIT_CRITICAL();
}

void LedON(void)
{
    while(1)
    {
        LedSwitch(BULE_GPIO_Pin,0);
        LedSwitch(GREEN_GPIO_Pin,1);
        printf("green_LED µ∆¡¡\r\n");
        vTaskDelay(1000);
    }
    
}

void LedOFF(void)
{
    while(1)
    {
        LedSwitch(GREEN_GPIO_Pin,0);
        LedSwitch(BULE_GPIO_Pin, 1);
        printf("BULE_LED µ∆¡¡\r\n");
        vTaskDelay(10);
    }
}