/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <Math.h>
#include <String.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define BUFFERSIZE 10

uint8_t command[BUFFERSIZE];
bool done = false;

//dont seem to work in the functions for some reason
uint8_t sof = '-';
uint8_t sep = ',';
uint8_t eof = ';';

void print_help(){ //-help for all commands, works
	char *help = "ALL COMMANDS AND STRUCTURE:\n"
			"\rSum:\n"
			"\r		-s, <x1> : <x2> ;"
			"\r\nDifference:\n"
			"\r		-d, <x1> : <x2> ;\n"
			"\rHelp page:\n"
			"\r		-h;\n";

	int help_len = strlen(help);
	HAL_UART_Transmit(&huart2,(uint8_t*)help, help_len, 1000);
}

void sum(){ //doesnt really work
	char *tmp = strdup((char*)command);

	//had to use the actual characters, gave me error messages and warnings if i used the variables i made, dont know why
	char *a_tmp = strstr(tmp, ","); //everything after :
	char *a = strtok(a_tmp, ":"); //everything before : til of a_tmp

	char *b_tmp = strstr(tmp, ":"); //same as before
	char *b = strtok(b_tmp, ";");

	//couldnt figure out how to get rid of the , and the ; so i hard coded it:
//	memmove(a, a-1, strlen(a));
//	memmove(b, b-1, strlen(b));

	int num1 = atoi(a);
	int num2 = atoi(b);

	int res = num1+num2;

	uint8_t *out = res;
	int len = strlen(out);

	HAL_UART_Transmit(&huart2, out, len, 1000);
}

void subtraction(){ //doesnt really work
	char *tmp = strdup((char*)command);

	char *a_tmp = strstr(tmp, ","); //everything after :
	char *a = strtok(a_tmp, ":"); //everything before : til of a_tmp

	char *b_tmp = strstr(tmp, ":"); //same as before
	char *b = strtok(b_tmp, ";");

	//couldnt figure out how to get rid of the , and the ; so i hard coded it:
//	memmove(a, a-1, strlen(a));
//	memmove(b, b-1, strlen(b));

	int num1 = atoi(a);
	int num2 = atoi(b);

	int res = num1-num2;

	uint8_t *out = res;
	int len = strlen(out);

	HAL_UART_Transmit(&huart2, out, len, 1000);

}

void syntax_error(){ //works
	char *errormsg = "There seems to be something wrong with your syntax, use -h to see all commands"
			"\r\nYour command:";

	int length = strlen(errormsg);
	if(HAL_UART_Transmit(&huart2, (uint8_t*)errormsg, length, 1000) == HAL_ERROR){
		Error_Handler();
	}
	if(HAL_UART_Transmit(&huart2, command, BUFFERSIZE, 1000) == HAL_ERROR){
		Error_Handler();
	}
}

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  char *commandMsg = "\r\n xD > ";
  char *confirmMsg = "\r\n #: ";

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //the only functions that work properly are the help and syntax_error
	  //issues i had with the input:
	  /*
	   * 1. couldnt make the input show on putty, not sure if this is a personal issue or just the way putty-uart functions
	   * 2. couldnt correctly cut the numbers out of the input string
	   * 2.1 full number wasnt being taken by either strtok or strstr
	   * 2.2 if memmove was used single digit numbers wouldnt appear
	   */

	  //i tried to do the input with a non set length but that didnt work
	  //demo shown in class only used a set buffersize and didnt show us how to do it without a set size

	  if(HAL_UART_Transmit(&huart2, (uint8_t*)commandMsg, strlen(commandMsg), 1000) == HAL_ERROR){
		  Error_Handler();
	  }
	  if(HAL_UART_Receive_IT(&huart2, command, BUFFERSIZE) == HAL_ERROR){
		  Error_Handler();
	  }
	  while(done == false)
		  ;
	  if(HAL_UART_Transmit(&huart2, (uint8_t*)confirmMsg, strlen(confirmMsg), 1000) == HAL_ERROR){
		  Error_Handler();
	  }

	  if(command[0] == sof){
		  switch(command[1]){
			  case ('h'):
				  print_help();
				  if(HAL_UART_Transmit(&huart2, command, BUFFERSIZE, 1000) == HAL_ERROR){
					  Error_Handler();
				  }
				  break;
			  case ('s'):
				  sum();
			  	  if(HAL_UART_Transmit(&huart2, command, BUFFERSIZE, 1000) == HAL_ERROR){
			  		  Error_Handler();
			  	  }
				  break;
			  case ('d'):
				  subtraction();
				  if(HAL_UART_Transmit(&huart2, command, BUFFERSIZE, 1000) == HAL_ERROR){
					  Error_Handler();
				  }
				  break;
			  default:
				  syntax_error();
				  break;
		  }
	  } else{
		  syntax_error();
	  }

	  done = false;
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	done = true;
}

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
