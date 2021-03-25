#include "modbus_time.h"

void Modbus_Time_Init()
{

}

// Modbus 定时器中断函数 1ms中断一次
void Modbus_TIM_IRQHandler(void)  
{
//	if (TIM_GetITStatus(Modbus_Timer, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//	{
//		TIM_ClearITPendingBit(Modbus_Timer, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
//		if(modbus.timrun != 0)
//		 {
//		  modbus.timout++;
//		  if(modbus.timout >=8)
//		  {
//		   modbus.timrun = 0;
//			 modbus.reflag = 1;
//		  }
//		 }			
//	}
}
