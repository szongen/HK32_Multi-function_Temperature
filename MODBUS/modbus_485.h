#ifndef _modbus_485_h_
#define _modbus_485_h_
#include "main.h"

// modbusÐÅºÅ¿ØÖÆ¶Ë¿Ú 
#define Modbus_485_RT_Port    GPIOC
#define Modbus_485_RT_Pin     GPIO_Pin_2
#define Modbus_485_RT_Clock   RCC_APB2Periph_GPIOC

#define Modbus_485_RX_Mode    GPIO_ResetBits(Modbus_485_RT_Port,Modbus_485_RT_Pin)
#define Modbus_485_TX_Mode    GPIO_SetBits(Modbus_485_RT_Port,Modbus_485_RT_Pin)

void Modbus_485_Init(void);

#endif

