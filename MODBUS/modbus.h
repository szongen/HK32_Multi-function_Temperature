#ifndef _modbus_h_
#define _modbus_h_
#include "main.h"
#include "modbus_crc.h"
#include "modbus_uart.h"
#include "modbus_485.h"
#include "modbus_time.h"


typedef struct 
{
  u8  myadd;        //本设备从机地址
	u8  rcbuf[100];   //modbus接受缓冲区
	u8  timout;       //modbus数据持续时间
	u8  recount;      //modbus端口接收到的数据个数
	u8  timrun;       //modbus定时器是否计时标志
	u8  reflag;       //modbus一帧数据接受完成标志位
	u8  sendbuf[100]; //modbus接发送缓冲区
	
}MODBUS;

extern MODBUS modbus;
extern u16 Reg[];
void Modbus_Init(void);
void Modbus_Func3(void);
void Modbus_Event(void);
void ISR_Init(void);

#endif

