#include "lis2dh12.h"
#include "myiic.h"

u8 LIS2DH12_Bus_Write(u8 Register_Address, u8 Word_Data)
{
	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */
	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(LIS2DH12_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第4步：发送字节地址 */
	IIC_Send_Byte(Register_Address);	// LIS不一样的地方！！！
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}	
	/* 第5步：开始写入数据 */
	IIC_Send_Byte(Word_Data);
	/* 第6步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 发送I2C总线停止信号 */
	IIC_Stop();
	return 1;	/* 执行成功 */
cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	IIC_Stop();
	return 0;
}

u8 LIS2DH12_Bus_Read(u8 Register_Address)
{
	u8  data;
	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(LIS2DH12_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第4步：发送字节地址， */
	IIC_Send_Byte(Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	IIC_Start();
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(LIS2DH12_WR_address | I2C_RD);	/* 此处是读指令 */
	/* 第8步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第9步：读取数据 */
	{
		data = IIC_Read_Byte(0);	/* 读1个字节 */
		IIC_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	}
	/* 发送I2C总线停止信号 */
	IIC_Stop();
	return data;	/* 执行成功 返回data值 */
cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	IIC_Stop();
	return 0;
}

void LIS2DH12_GetAccAxesRaw(u8* Data)
{
  u8 i, j, k;
  u8 numberOfByteForDimension;
  
  numberOfByteForDimension = 6/3;

  k=0;
  for (i=0; i<3;i++ ) 
  {
	for (j=0; j<numberOfByteForDimension;j++ )
	{	
		Data[k] = LIS2DH12_Bus_Read(LIS2DH12_OUT_X_L+k);
		k++;	
	}
  }
 
}

//LIS2DH12初始化
void LIS2DH12_init()
{	
	//IIC_Init();	//需移动？？在max中已经初始化
	//LIS2DH12_reset();	//不会
	LIS2DH12_Bus_Write(LIS_CTRL_REG1, 0x57); // 100Hz, 高分辨率，xyz使能
	//LIS2DH12_Bus_Write(LIS_CTRL_REG2, 0x57); // 内部滤波器配置（不会）
	//LIS2DH12_Bus_Write(LIS_CTRL_REG3, 0x02); // FIFO溢出中断
	LIS2DH12_Bus_Write(LIS_CTRL_REG4, 0x10); // 4g，正常模式10bit，数字1代表4/512=7.8125mg
	//LIS2DH12_Bus_Write(LIS_CTRL_REG5, 0x40); // 开启FIFO先进先出
	//LIS2DH12_Bus_Write(LIS_FIFO_CTRL_REG, 0xC0); // 流至先进先出模式
}
//LIS2DH12复位（不会？）
//void LIS2DH12_reset(void)
//{
//	LIS2DH12_Bus_Write(LIS_MODE_CONFIG,0x40);
//	LIS2DH12_Bus_Write(LIS_MODE_CONFIG,0x40);
//}

//void LIS2DH12_FIFO_ReadBytes(u8 Register_Address, u8* Data)
//{	
////	LIS2DH12_Bus_Read(LIS_INTR_STATUS_1);	// 清除中断标志？？
////	LIS2DH12_Bus_Read(LIS_INTR_STATUS_2);	
//	/* 第1步：发起I2C总线启动信号 */
//	IIC_Start();
//	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	IIC_Send_Byte(LIS2DH12_WR_address | I2C_WR);	/* 此处是写指令 */
//	/* 第3步：发送ACK */
//	if (IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}
//	/* 第4步：发送字节地址， */
//	IIC_Send_Byte(Register_Address + 0x80);
//	if (IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}
//	/* 第6步：重新启动I2C总线。下面开始读取数据 */
//	IIC_Start();
//	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	IIC_Send_Byte(LIS2DH12_WR_address | I2C_RD);	/* 此处是读指令 */
//	/* 第8步：发送ACK */
//	if (IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}
//	/* 第9步：读取数据 */
//	Data[0] = IIC_Read_Byte(1);	
//	Data[1] = IIC_Read_Byte(1);	
//	Data[2] = IIC_Read_Byte(1);		// 读取几个数据？是否需要增加
//	Data[3] = IIC_Read_Byte(1);
//	Data[4] = IIC_Read_Byte(1);	
//	Data[5] = IIC_Read_Byte(1);
//	Data[6] = IIC_Read_Byte(0);
////	Data[7] = IIC_Read_Byte(1);	
////	Data[8] = IIC_Read_Byte(0);
//	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
//	/* 发送I2C总线停止信号 */
//	IIC_Stop();
//cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
//	/* 发送I2C总线停止信号 */
//	IIC_Stop();
//}
