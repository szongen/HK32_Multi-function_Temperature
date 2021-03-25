/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "wifi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DHT11_Sensor        //使用DHT11传感器                             
//#define SHT30_Sensor        //使用SHT30传感器
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t uart_temp,RxLine=0,rs485_buffer[200];
DHT11_Data_TypeDef DHT11;
RTC_DateTypeDef GetData;  //获取日期结构体
RTC_TimeTypeDef GetTime;   //获取时间结构体

RTC_DateTypeDef GetData1;  //获取日期结构体
RTC_TimeTypeDef GetTime1;   //获取时间结构体

uint8_t WIFI_Connect_FLAG = 0;
uint8_t Rtc_Flag = 0;

uint32_t Sys_time = 0;     //时钟基准
uint32_t task1_time = 0;   //任务1 DHT11时钟
uint32_t task2_time = 0;   //任务2 网络授时时钟
uint32_t task3_time = 0;   //任务3 数码管显示时钟
uint32_t uart_time = 0;   //任务4 

char buffer[6] ={'0','1','2','3','4','5'};


#define MAX_RECV_LEN 1024         //设定可以接收的最大字节
uint8_t msg_buff[MAX_RECV_LEN] = {0}; //接收缓存区
uint8_t * msg = msg_buff;    //定义一个指针指向接收缓存区
int flag = 0;        //接收完成标志
int len_u4=0;  //数据长度记录

uint16_t aa;
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
	uint8_t key;
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
  MX_RTC_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  wifi_protocol_init();
  HAL_UART_Receive_IT(&huart1,&uart_temp,1);
  delay_init(48);
  TM1620_Init();
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET); //RS485控制流
  HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    wifi_uart_service();
    if(mcu_get_wifi_work_state()!=WIFI_CONN_CLOUD)
    {
        WIFI_Connect_FLAG = 0;
    }
    else
    {
        WIFI_Connect_FLAG = 1;
    }
      
/*--------------------------------------------------------------------------------------------------------------------------------------*/
//按键任务    
    
    key = KEY_Scan(0);
    switch(key)
    {
        case KEY1_PRES:
             mcu_set_wifi_mode(0);       
            break;
        case KEY2_PRES:
            Rtc_Flag++;
        if(Rtc_Flag >= 4)
			{
				Rtc_Flag = 0;
			}
			break;
			
			default: break;
    }


/*--------------------------------------------------------------------------------------------------------------------------------------*/
//Task1 温湿度采集任务    
#ifdef DHT11_Sensor     
      if((Sys_time-task1_time)>=100||task1_time==0)
      {
          DHT11_ReadData(&DHT11);

          if(WIFI_Connect_FLAG == 1)
          {
              mcu_dp_value_update(DPID_TEMP_CURRENT,(DHT11.temp_int*10+DHT11.temp_deci)); //温度数据上报;上传浮点型数据需将数据乘以10倍
              mcu_dp_value_update(DPID_HUMIDITY_VALUE,(DHT11.humi_int)); //湿度数据上报;
          }
          task1_time = Sys_time;
      }
#endif

      
#ifdef SHT30_Sensor
      
      
      
      
      
#endif
          
/*--------------------------------------------------------------------------------------------------------------------------------------*/      
//Task2 RTC网络授时
  if(WIFI_Connect_FLAG ==1)
  {
      if((Sys_time-task2_time)>=3000||task2_time==0)
      {
          mcu_get_system_time();
          task2_time = Sys_time;
      }
  }
  
/*--------------------------------------------------------------------------------------------------------------------------------------*/ 
/*RTC_Flag = 0时，显示温湿度
  RTC_FLAG = 1时，显示时间
  RTC_FLAG = 2时，显示日期
  RCT_FLAG = 3时，显示温湿度 ，第三个小数点为点亮，设备为MODBUS传感器
*/
//Task3 数码管任务
  if((Sys_time-task3_time)>=100||task3_time==0)
  {
      HAL_RTC_GetTime(&hrtc, &GetTime1, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &GetData1, RTC_FORMAT_BIN);
      if(Rtc_Flag == 0||Rtc_Flag == 3)
      {
          sprintf(buffer,"%2d%1d%2d%1d",DHT11.temp_int,DHT11.temp_deci,DHT11.humi_int,DHT11.humi_deci);
          
      }
      if(Rtc_Flag == 1)
      {
          sprintf(buffer,"%2d%1d%1d%2d",GetTime.Hours,0,0,GetTime1.Minutes);
      }
      if(Rtc_Flag == 2)
      {
          sprintf(buffer,"%2d%1d%1d%2d",GetData1.Month,0,0,GetData1.Date);
      }
      TM1620_Print(buffer);
      task3_time = Sys_time;
  }
/*--------------------------------------------------------------------------------------------------------------------------------------*/    
//rs485任务
  if(Rtc_Flag==3)
  {
        if (flag == 1)
        {
//			aa=CheckCRC(msg_buff,6)&0xff00;
//			if(msg_buff[0]==0x01&&msg_buff[1]==0x03)
//			{
//				HAL_GPIO_WritePin(USART_RE_GPIO_Port,USART_RE_Pin,GPIO_PIN_SET);
//				printf("%s",msg_buff);
//				HAL_GPIO_WritePin(USART_RE_GPIO_Port,USART_RE_Pin,GPIO_PIN_SET);
//			}
//            delay_ms(10); //加延时，保证接收到数据过长的时候，等待数据存入缓存区发送
////            HAL_UART_Transmit(&huart1,msg_buff, len_u4,100);     //将串口4接收到的数据通过串口3传出
//            memset(msg_buff, 0, sizeof(msg_buff));   //清空缓存区
//            // 指向接收缓存的头部
//            msg = msg_buff;
//            (&huart1)->pRxBuffPtr = msg;
//            flag = 0;
//            len_u4=0;//每次数据长度清0
        }
  }
  
  
  

  
//      /* Display date Format : yy/mm/dd */
//      SEGGER_RTT_printf(0,"%02d/%02d/%02d\r\n",2000 + GetData1.Year, GetData1.Month, GetData1.Date);
//      /* Display time Format : hh:mm:ss */
//      SEGGER_RTT_printf(0,"%02d:%02d:%02d\r\n",GetTime1.Hours, GetTime1.Minutes, GetTime1.Seconds);

      
      
      
/*--------------------------------------------------------------------------------------------------------------------------------------*/
      
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(Rtc_Flag ==3)
    {
        if(huart->Instance == huart1.Instance)
            {
                uart_time = Sys_time;
				uint8_t ret = HAL_OK;
				msg++;
				len_u4++;//数据长度计数
				if( msg == msg_buff + MAX_RECV_LEN)
				{
					msg = msg_buff;
				}
				do
				{
					ret = HAL_UART_Receive_IT(&huart1,(uint8_t *)msg,1);
				} while(ret != HAL_OK);
				if(*(msg-1) == '\n')   //接收以\n为结尾字符，则表示接收完成
				{
					flag  = 1;
				}
		}
	}
    else
    {
    if (huart->Instance == huart1.Instance){
        uart_receive_input(uart_temp);
    }
    HAL_UART_Receive_IT(huart,&uart_temp,1);
    }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim->Instance == TIM1){

        /* Toggle LED */
        Sys_time++;
    }

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
