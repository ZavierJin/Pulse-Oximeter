#ifndef __MYIIC_H
#define __MYIIC_H

//#ifndef __MAX30102_H
//#define __MAX30102_H

#include "nrf_delay.h"
#include "nrf52.h"
  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//IIC 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/10 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

#define  delay_us    nrf_delay_us 
#define  delay_ms    nrf_delay_ms 
#define  u8          uint8_t 

#define  IIC_SCL     16//26//4
#define  IIC_SDA     15//25//5
#define  MAX_DRDY    31//31//23//15		// 必须要int口??
#define  LIS_DRDY    30//23//15
   	   		   
////IO方向设置
//#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=4;}	
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=7;}

////IO操作函数	 
//#define IIC_SCL    PBout(7) //SCL
//#define IIC_SDA    PBout(8) //SDA	 
//#define READ_SDA   PBin(8)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号   
void IIC_NAck(void);				//IIC不发送ACK信号
void IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength);
void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength);
#endif
















