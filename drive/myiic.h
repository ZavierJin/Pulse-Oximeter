#ifndef __MYIIC_H
#define __MYIIC_H

//#ifndef __MAX30102_H
//#define __MAX30102_H

#include "nrf_delay.h"
#include "nrf52.h"
  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/10 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

#define  delay_us    nrf_delay_us 
#define  delay_ms    nrf_delay_ms 
#define  u8          uint8_t 

#define  IIC_SCL     16//26//4
#define  IIC_SDA     15//25//5
#define  MAX_DRDY    31//31//23//15		// ����Ҫint�ڡ?
#define  LIS_DRDY    30//23//15
   	   		   
////IO��������
//#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=4;}	
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=7;}

////IO��������	 
//#define IIC_SCL    PBout(7) //SCL
//#define IIC_SDA    PBout(8) //SDA	 
//#define READ_SDA   PBin(8)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�   
void IIC_NAck(void);				//IIC������ACK�ź�
void IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength);
void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength);
#endif
















