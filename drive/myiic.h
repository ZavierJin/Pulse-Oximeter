#ifndef __MYIIC_H
#define __MYIIC_H

//#ifndef __MAX30102_H
//#define __MAX30102_H

#include "nrf_delay.h"
#include "nrf52.h"
  
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//Mini STM32¿ª·¢°å
//IIC Çý¶¯º¯Êý	   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ÐÞ¸ÄÈÕÆÚ:2010/6/10 
//°æ±¾£ºV1.0
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÕýµãÔ­×Ó 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

#define  delay_us    nrf_delay_us 
#define  delay_ms    nrf_delay_ms 
#define  u8          uint8_t 

#define  IIC_SCL     16//26//4
#define  IIC_SDA     15//25//5
#define  MAX_DRDY    31//31//23//15		// ±ØÐëÒªint¿Ú¡?
#define  LIS_DRDY    30//23//15
   	   		   
////IO·½ÏòÉèÖÃ
//#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=4;}	
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=7;}

////IO²Ù×÷º¯Êý	 
//#define IIC_SCL    PBout(7) //SCL
//#define IIC_SDA    PBout(8) //SDA	 
//#define READ_SDA   PBin(8)  //ÊäÈëSDA 

//IICËùÓÐ²Ù×÷º¯Êý
void IIC_Init(void);                //³õÊ¼»¯IICµÄIO¿Ú				 
void IIC_Start(void);				//·¢ËÍIIC¿ªÊ¼ÐÅºÅ
void IIC_Stop(void);	  			//·¢ËÍIICÍ£Ö¹ÐÅºÅ
void IIC_Send_Byte(u8 txd);			//IIC·¢ËÍÒ»¸ö×Ö½Ú
u8 IIC_Read_Byte(unsigned char ack);//IIC¶ÁÈ¡Ò»¸ö×Ö½Ú
u8 IIC_Wait_Ack(void); 				//IICµÈ´ýACKÐÅºÅ
void IIC_Ack(void);					//IIC·¢ËÍACKÐÅºÅ   
void IIC_NAck(void);				//IIC²»·¢ËÍACKÐÅºÅ
void IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength);
void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength);
#endif
















