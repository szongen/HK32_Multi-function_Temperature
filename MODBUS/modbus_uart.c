#include "modbus_uart.h"

// Modbus串口初始化
void Modbus_Uart_Init()
{

}

//modbus串口发送一个字节数据
void Modbus_Send_Byte(u8 Modbus_byte)
{
//  USART_SendData(Modbus_USART,Modbus_byte);
//	while(USART_GetFlagStatus(Modbus_USART, USART_FLAG_TC) == RESET);
//	USART_ClearFlag(Modbus_USART, USART_FLAG_TC); 
}

//modbus串口中断服务程序
void Modbus_USART_IRQHandler(void)                
{
//  u8 st,Res;
//	st = USART_GetITStatus(Modbus_USART, USART_IT_RXNE);
//	if(st == SET)//接收中断
//	{
//		Res =USART_ReceiveData(Modbus_USART);	//读取接收到的数据
//	  
//	 if( modbus.reflag==1)  //有数据包正在处理
//	  {
//		   return ;
//		}		
//	  modbus.rcbuf[modbus.recount++] = Res;
//		modbus.timout = 0;
//		if(modbus.recount == 1)  //已经收到了第二个字符数据
//		{
//		  modbus.timrun = 1;  //开启modbus定时器计时
//		}
//	}	
} 
