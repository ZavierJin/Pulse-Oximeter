
#ifndef __LIS2DH12_H
#define __LIS2DH12_H
	  
#include "stdint.h"

#define  u8       uint8_t 

#define LIS2DH12_INT PBin(2)


#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

#define LIS2DH12_WR_address 0x32 //or 0x32??

//#define I2C_WRITE_ADDR 0xAE
//#define I2C_READ_ADDR 0xAF

////register addresses
//#define LIS_INTR_STATUS_1 0x00
//#define LIS_INTR_STATUS_2 0x01
//#define LIS_INTR_ENABLE_1 0x02
//#define LIS_INTR_ENABLE_2 0x03
//#define LIS_FIFO_WR_PTR 0x04
//#define LIS_OVF_COUNTER 0x05
//#define LIS_FIFO_RD_PTR 0x06
//#define LIS_FIFO_DATA 0x07
//#define LIS_FIFO_CONFIG 0x08
//#define LIS_MODE_CONFIG 0x09
//#define LIS_SPO2_CONFIG 0x0A
//#define LIS_LED1_PA 0x0C	// See datasheet, page 21
//#define LIS_LED2_PA 0x0D
//#define LIS_LED3_PA 0x0E
//#define LIS_PILOT_PA 0x10
//#define LIS_MULTI_LED_CTRL1 0x11
//#define LIS_MULTI_LED_CTRL2 0x12
//#define LIS_TEMP_INTR 0x1F
//#define LIS_TEMP_FRAC 0x20
//#define LIS_TEMP_CONFIG 0x21
//#define LIS_PROX_INT_THRESH 0x30
//#define LIS_REV_ID 0xFE
//#define LIS_PART_ID 0xFF

#define LIS_CTRL_REG1 0x20
#define LIS_CTRL_REG2 0x21
#define LIS_CTRL_REG3 0x22
#define LIS_CTRL_REG4 0x23
#define LIS_CTRL_REG5 0x24
#define LIS_FIFO_CTRL_REG 0x2E
#define LIS2DH12_OUT_X_L 0x28


void LIS2DH12_init(void);  
//void LIS2DH12_reset(void);
//void LIS2DH12_FIFO_ReadBytes(u8 Register_Address,u8* Data);
void LIS2DH12_GetAccAxesRaw(u8* Data);
#endif
















