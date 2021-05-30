#include "main.h"
#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "delay.h"
#include "lan8720.h"
#include "pcf8574.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "udp_demo.h"
#include "lwip_tcp_demo.h"
#include "queue.h"
#include "Semphr.h"
#include "timers.h"
void SystemClock_Config(void);                //时钟配置函数 180M
static void MX_GPIO_Init(void);

#define VERSION                     ("1.0.2")

#define SKY_STARTTASK_PRIO             2                   //开始任务优先级
#define SKY_APPTASK_PPRIO              7


#define SKY_STARTTASK_SIZE             128
#define SKY_APPTASK_SIZE               128*4

#define SKY_MQTTTIMERHEART_PERIOD      1000*56              //56s

//任务句柄
TaskHandle_t    SkyStartTask_Handler;
TaskHandle_t    SkyAppTask_Handler;

//定时器句柄
TimerHandle_t   Sky_MqttHeartTimer;



//线程回调函数
static void Sky_StartThread(void *parameters);
static void Sky_AppThread(void *parameters);
//定时器回调函数
static void Sky_TimerCallback(void *para);

static void Sky_BoardInit(void);

int main(void)
{
    Sky_BoardInit();
    printf("STM32F4-FreeRTOS Version :v%s\r\n",VERSION);
    
    xTaskCreate((TaskFunction_t )Sky_StartThread,
                (const char *) "starttask",
                (uint16_t      )SKY_STARTTASK_SIZE,
                (void *)        NULL,
                (UBaseType_t   )SKY_STARTTASK_PRIO,
                (TaskHandle_t *)&SkyStartTask_Handler);
                
     vTaskStartScheduler();
  /* USER CODE END 3 */
}

/***
*@beif  Sky_BoardInit
*@para  void
*@data  2021-5-30
*@auth  GECHENG
*@note  硬件初始化
*/
void Sky_BoardInit(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    key_init();
    led_init();
    uart1_init();
    PCF8574_Init();
}

/***
*@beif  Sky_StartThread
*@para  void
*@data  2021-5-30
*@auth  GECHENG
*@note  系统初始化线程
*/
void Sky_StartThread(void *parameters)
{
    while(lwip_comm_init())                                                         //初始化LWIP协议
    {
        printf("LWIP init fail\r\n");
    }
    printf("LWIP init Success\r\n");

    Sky_TcpClientThread_Init();                                                              //创建TCP客户端线程
#if LWIP_DHCP
    lwip_comm_dhcp_creat();
#endif


    
    Sky_MqttHeartTimer=xTimerCreate("mqtt heart",SKY_MQTTTIMERHEART_PERIOD,pdTRUE,0,Sky_TimerCallback);                //创建心跳定时器 56ms
    
    xTaskCreate((TaskFunction_t )Sky_AppThread,
                (const char *   )"apptask",
                (uint16_t       )SKY_APPTASK_SIZE,
                (void *)         NULL,
                (UBaseType_t    )SKY_APPTASK_PPRIO,
                (TaskHandle_t * )&SkyAppTask_Handler);
                
                
    xTimerStart(Sky_MqttHeartTimer,0);                                                          //开启心跳定时器
                
    vTaskDelete(SkyStartTask_Handler);                                                          //删除开始线程
                
    vTaskStartScheduler();
}

/***
*@beif  Sky_StartThread
*@para  void
*@data  2021-5-30
*@auth  GECHENG
*@note  系统初始化线程
*/
void Sky_AppThread(void *parameters)
{

    uint8_t *buffer;
    uint8_t key=0;
    
    while(1)
    {
        key =Sky_KeyScan(0);
        switch (key)
        {
            case KEY0_PRES:
                LEDRTOGG;
                break;
        }
        printf(" SKY_APP THRAD IS running\r\n");
        vTaskDelay(100);
    }
    
}
/**
*@beif  Sky_TimerCallback
*/
void Sky_TimerCallback(void *para)
{
    if(mqtt_alive ==1)
    {
        LEDG(1);
        mqtt_sendheart();
    }
    printf("MQTT Heart TIMER is running\r\n");
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  __HAL_RCC_GPIOH_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
