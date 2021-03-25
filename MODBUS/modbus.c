#include "modbus.h"

MODBUS modbus;
u16 Reg[] ={0x0001,
            0x0002,
            0x0003,
            0x0004,
	          0x0005,
            0x0006,
            0x0007,
           };
// Modbus初始化函数
void Modbus_Init()
{
  modbus.myadd = 0x01; //从机设备地址为1
  modbus.timrun = 0;    //modbus定时器停止计算
	Modbus_485_Init();
	Modbus_Uart_Init();
	Modbus_Time_Init();
}

// Modbus 3号功能码函数
// Modbus 主机读取寄存器值
void Modbus_Func3()
{
  u16 Regadd,Reglen,crc;
	u8 i,j;	
	//得到要读取寄存器的首地址
	Regadd = modbus.rcbuf[2]*256+modbus.rcbuf[3];
	//得到要读取寄存器的数据长度
	Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];
	//发送回应数据包
	i = 0;
	modbus.sendbuf[i++] = modbus.myadd;      //发送本机设备地址
	modbus.sendbuf[i++] = 0x03;              //发送功能码
  modbus.sendbuf[i++] = ((Reglen*2)%256);   //返回字节个数
	for(j=0;j<Reglen;j++)                    //返回数据
	{
	  modbus.sendbuf[i++] = Reg[Regadd+j]/256;
		modbus.sendbuf[i++] = Reg[Regadd+j]%256;
	}
	crc = Modbus_CRC16(modbus.sendbuf,i);    //计算要返回数据的CRC
	modbus.sendbuf[i++] = crc/256;
	modbus.sendbuf[i++] = crc%256;
	// 开始返回Modbus数据
	Modbus_485_TX_Mode;
	for(j=0;j<i;j++)
	{
	  Modbus_Send_Byte(modbus.sendbuf[j]);	
	}
	Modbus_485_RX_Mode;
}
// Modbus 6号功能码函数
// Modbus 主机写入寄存器值
void Modbus_Func6()  
{
  u16 Regadd;
	u16 val;
	u16 i,crc,j;
	i=0;
  Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //得到要修改的地址 
	val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //修改后的值
	Reg[Regadd]=val;  //修改本设备相应的寄存器
	
	//以下为回应主机
	modbus.sendbuf[i++]=modbus.myadd;//本设备地址
  modbus.sendbuf[i++]=0x06;        //功能码 
  modbus.sendbuf[i++]=Regadd/256;
	modbus.sendbuf[i++]=Regadd%256;
	modbus.sendbuf[i++]=val/256;
	modbus.sendbuf[i++]=val%256;
	crc=Modbus_CRC16(modbus.sendbuf,i);
	modbus.sendbuf[i++]=crc/256;  //
	modbus.sendbuf[i++]=crc%256;
	
	Modbus_485_TX_Mode;  
	for(j=0;j<i;j++)
	{
	 Modbus_Send_Byte(modbus.sendbuf[j]);
	}
	Modbus_485_RX_Mode;
}

// Modbus事件处理函数
void Modbus_Event()
{
	u16 crc,rccrc;
	//没有收到数据包
  if(modbus.reflag == 0)  
	{
	   return;
	}
	//收到数据包
	//通过读到的数据帧计算CRC
	crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount-2); 
	// 读取数据帧的CRC
	rccrc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];
	if(crc == rccrc) //CRC检验成功 开始分析包
	{	
	   if(modbus.rcbuf[0] == modbus.myadd)  // 检查地址是否时自己的地址
		 {
		   switch(modbus.rcbuf[1])   //分析modbus功能码
			 {
			   case 0:             break;
				 case 1:             break;
				 case 2:             break;
				 case 3:      Modbus_Func3();      break;
				 case 4:             break;
				 case 5:             break;
         case 6:      Modbus_Func6();      break;
				 case 7:             break;
				 case 8:             break;
				 case 9:             break;			 
			 }
		 }
		 else if(modbus.rcbuf[0] == 0) //广播地址不予回应
		 {
		    
		 }	 
	}	
	 modbus.recount = 0;
   modbus.reflag = 0; 
}

// Modbus_TIM Modbus_USART中断函数配置
void ISR_Init()
{

}




