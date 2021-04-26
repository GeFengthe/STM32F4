#include "main.h"
#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
void SystemClock_Config(void);                // ±÷”≈‰÷√∫Ø ˝ 180M
static void MX_GPIO_Init(void);

#define START_TASK_PRIO             1
#define APP_TASK_PPRIO              3
#define MESS_TASK_PRIO              4


#define START_TASK_SIZE             128
#define APP_TASK_SIZE               128*4
#define MESS_TASK_SIZE              128*4

TaskHandle_t    StartTask_Handler;
TaskHandle_t    AppTask_Handler;
TaskHandle_t    MessTask_Handler;

static void StartTask(void *parameters);
static void AppTask(void *parameters);

//xTaskHandle


int main(void)
{
    HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
    SystemClock_Config();
    MX_GPIO_Init();
    xTaskCreate((TaskFunction_t )StartTask,
                (const char *) "starttask",
                (uint16_t      )START_TASK_SIZE,
                (void *)        NULL,
                (UBaseType_t   )START_TASK_PRIO,
                (TaskHandle_t *)&StartTask_Handler);
                
     vTaskStartScheduler();

    while (1)
    {
        
        
        
    }
  /* USER CODE END 3 */
}

void StartTask(void *parameters)
{
    led_init();
    uart1_init();
    xTaskCreate((TaskFunction_t )AppTask,
                (const char *   )"apptask",
                (uint16_t       )APP_TASK_SIZE,
                (void *)         NULL,
                (UBaseType_t    )APP_TASK_PPRIO,
                (TaskHandle_t * )&AppTask_Handler);
                
    vTaskDelete(StartTask_Handler);
    vTaskStartScheduler();
}

void AppTask(void *parameters)
{
    static uint8_t ledtog =0;
    while(1)
    {
        printf("time----------\r\n");
        if(ledtog ==0)
        {
            LEDR(0);
            ledtog =1;
        }else
        {
            LEDR(1);
            ledtog =0;
        }
        vTaskDelay(1000);
    }
    
}

void MessTask(void *parameters)
{
    
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
