#include "max30102.h"
#include "myiic.h"

u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data)
{
	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */
	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第4步：发送字节地址 */
	IIC_Send_Byte(Register_Address);
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

u8 max30102_Bus_Read(u8 Register_Address)
{
	u8  data;
	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第4步：发送字节地址， */
	IIC_Send_Byte((uint8_t)Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	IIC_Start();
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */
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

void max30102_FIFO_ReadBytes(u8 Register_Address,u8* Data)
{	
	max30102_Bus_Read(REG_INTR_STATUS_1);
	max30102_Bus_Read(REG_INTR_STATUS_2);	
	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第4步：发送字节地址， */
	IIC_Send_Byte((uint8_t)Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	IIC_Start();
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */
	/* 第8步：发送ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第9步：读取数据 */
//	int j,i;
//	int j_max = 1;		// 读取样本个数
//	int i_max = 3;		// LED数
//	for(j=0; j<j_max; ++j){
//		for(i=0; i<i_max; ++i){
//			Data[9*j+3*i] = IIC_Read_Byte(1);	
//			Data[9*j+3*i+1] = IIC_Read_Byte(1);		// 读取几个数据？是否需要增加
//			if(j==j_max-1 && i==i_max-1) Data[9*j+3*i+2] = IIC_Read_Byte(0);
//			else Data[9*j+3*i+2] = IIC_Read_Byte(1);
//		}
//	}
	Data[0] = IIC_Read_Byte(1);	
	Data[1] = IIC_Read_Byte(1);	
	Data[2] = IIC_Read_Byte(1);		// 读取几个数据？是否需要增加
	Data[3] = IIC_Read_Byte(1);
	Data[4] = IIC_Read_Byte(1);	
	Data[5] = IIC_Read_Byte(1);
	Data[6] = IIC_Read_Byte(1);
	Data[7] = IIC_Read_Byte(1);	
	Data[8] = IIC_Read_Byte(0);
	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	/* 发送I2C总线停止信号 */
	IIC_Stop();
cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	IIC_Stop();
}
//max30102初始化
void max30102_init()
{
	IIC_Init();
	max30102_reset();
	max30102_Bus_Write(REG_INTR_ENABLE_1,0x80);	// INTR setting---80
	max30102_Bus_Write(REG_INTR_ENABLE_2,0x00);
	max30102_Bus_Write(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
	max30102_Bus_Write(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
	max30102_Bus_Write(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
	max30102_Bus_Write(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17，P18
	max30102_Bus_Write(REG_MODE_CONFIG,0x03);  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	max30102_Bus_Write(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (411uS) ，P19 
	max30102_Bus_Write(REG_LED1_PA,0x44);   	//Choose value for ~ 7mA for LED1，P21
	max30102_Bus_Write(REG_LED2_PA,0x33);   	// Choose value for ~ 7mA for LED2
	//max30102_Bus_Write(REG_LED3_PA,0xff);   	// Choose value for ~ 7mA for LED3
	//max30102_Bus_Write(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED
	max30102_Bus_Write(REG_MULTI_LED_CTRL1,0x21);	// In multi-LED mode, which LED is active，P22
	max30102_Bus_Write(REG_MULTI_LED_CTRL2,0x03);	
// Finger: Red IR: 33
// Wrist up: Red 60, IR 40
// Wrist down: Red 80, IR 50; Red 55, IR 40
// 50,33 -> 37,36
}
//max30102复位
void max30102_reset(void)
{
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
}
