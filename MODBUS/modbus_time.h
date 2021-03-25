#ifndef _modbus_time_h_
#define _modbus_time_h_
#include "main.h"
#include "modbus.h"

/*************modbus默认定时器TIM3******************/
/*************可修改对应的宏来修改TIM****************/

#define Modbus_Time_arr               (1000-1) //1us*1000=1ms 
#define Modbus_Time_psc               (72-1)   //72M/72 =1us
#define Modbus_Time_Clock             RCC_APB1Periph_TIM3
#define Modbus_Timer                  TIM3
#define Modbus_Time_IRQn              TIM3_IRQn
#define Modbus_TIM_IRQHandler(void)   TIM3_IRQHandler(void)

void Modbus_Time_Init(void);
#endif 

