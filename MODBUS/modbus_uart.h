#ifndef _modbus_uart_h_
#define _modbus_uart_h_

#include "main.h"
#include "modbus.h"

#define Modbus_bound             9600
#define Modbus_USART             USART2
#define Modbus_Uart_Clock        RCC_APB1Periph_USART2
#define Modbus_Uart_GPIO_Clock   RCC_APB2Periph_GPIOA
#define Modbus_Uart_TRX_Port     GPIOA      
#define Modbus_Uart_TX_Pin       GPIO_Pin_2
#define Modbus_Uart_RX_Pin       GPIO_Pin_3

#define Modbus_USART_IRQn              USART2_IRQn
#define Modbus_USART_IRQHandler(void)  USART2_IRQHandler(void) 

void Modbus_Uart_Init(void);
void Modbus_Send_Byte(u8 Modbus_byte);

#endif 

