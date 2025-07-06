/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  /* 通过串口1发送“Hello” */
  HAL_UART_Transmit(&huart1, (uint8_t*)"Hello", 5, 0xFF);

//   /* 接收信息缓存，只接收5个字节数据 */
//   char recv_str[10];
//   HAL_UART_Receive(&huart1, recv_str, 5, 0xFFFFFFFF);

//   /* 将接收到的信息发回给电脑 */
//   HAL_UART_Transmit(&huart1, recv_str, 5, 0xFF);

  /* 输出变量 */
  char wave_out = 0;

  /* LED计时变量 */
  uint16_t led_time = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // /* 打印日志并输出变量, 输出“\r\n”是为了换行显示 */
    // PrintfDebug("LED Green State %d\r\n", HAL_GPIO_ReadPin(LED_G_GPIO_Port, LED_G_Pin));

    // /* 反转引脚状态 */
    // HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);

    // /* 延时500ms */
    // HAL_Delay(500);

    /* LED计时到500ms状态切换一次 */
    if(led_time >= 500)
    {
        led_time = 0; // 重新计时
        /* 翻转引脚状态 */
        HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
    }
    
    /* 隔5ms发送一次变量点 */
    if(wave_out >= 100)
    {
        /* 如果wave_out大于等于100，则输出由99 -> 0, 一个100个点 */
        /* 通过串口1发送三角波变量数据，并增加包头包尾用于解析数据 */
        HAL_UART_Transmit(&huart1, (uint8_t[]){0xD2, (199 - wave_out), 0xD3}, 3, 0xFF);
    }else{
        /* 如果wave_out小于100，则输出由0 -> 99, 一个100个点 */
        /* 通过串口1发送三角波变量数据，并增加包头包尾用于解析数据 */
        HAL_UART_Transmit(&huart1, (uint8_t[]){0xD2, wave_out, 0xD3}, 3, 0xFF);
    }

    /* 隔5ms发送一次变量点 */
    if(wave_out >= 100)
    {
        /* 如果wave_out大于等于100，则输出由0 -> 99, 一个100个点 */
        /* 通过串口1发送锯齿波变量数据，并增加包头包尾用于解析数据 */
        HAL_UART_Transmit(&huart1, (uint8_t[]){0xCE, (wave_out % 100), 0xCF}, 3, 0xFF);
    }else{
        /* 如果wave_out小于100，则输出由0 -> 99, 一个100个点 */
        /* 通过串口1发送锯齿波变量数据，并增加包头包尾用于解析数据 */
        HAL_UART_Transmit(&huart1, (uint8_t[]){0xCE, wave_out, 0xCF}, 3, 0xFF);
    }
    
    /**
     * wave_out在[0, 199]循环
     * 一个周期200个数据点，一个数据点5ms
     * 所以波形一个周期为1000ms
     */
    wave_out++;
    if(wave_out >= 200)
    {
        wave_out = 0;
    }

    /* 保持电平状态方便观察 */
    HAL_Delay(5); // 延时10ms
    led_time += 5; // LED计时增加

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
