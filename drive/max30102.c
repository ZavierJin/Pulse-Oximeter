#include "max30102.h"
#include "myiic.h"

u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data)
{
	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
	/* ��1��������I2C���������ź� */
	IIC_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��4���������ֽڵ�ַ */
	IIC_Send_Byte(Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}	
	/* ��5������ʼд������ */
	IIC_Send_Byte(Word_Data);
	/* ��6��������ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ����I2C����ֹͣ�ź� */
	IIC_Stop();
	return 1;	/* ִ�гɹ� */
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	IIC_Stop();
	return 0;
}

u8 max30102_Bus_Read(u8 Register_Address)
{
	u8  data;
	/* ��1��������I2C���������ź� */
	IIC_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��4���������ֽڵ�ַ�� */
	IIC_Send_Byte((uint8_t)Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	IIC_Start();
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
	/* ��8��������ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��9������ȡ���� */
	{
		data = IIC_Read_Byte(0);	/* ��1���ֽ� */
		IIC_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
	}
	/* ����I2C����ֹͣ�ź� */
	IIC_Stop();
	return data;	/* ִ�гɹ� ����dataֵ */
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	IIC_Stop();
	return 0;
}

void max30102_FIFO_ReadBytes(u8 Register_Address,u8* Data)
{	
	max30102_Bus_Read(REG_INTR_STATUS_1);
	max30102_Bus_Read(REG_INTR_STATUS_2);	
	/* ��1��������I2C���������ź� */
	IIC_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��4���������ֽڵ�ַ�� */
	IIC_Send_Byte((uint8_t)Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	IIC_Start();
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
	/* ��8��������ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��9������ȡ���� */
//	int j,i;
//	int j_max = 1;		// ��ȡ��������
//	int i_max = 3;		// LED��
//	for(j=0; j<j_max; ++j){
//		for(i=0; i<i_max; ++i){
//			Data[9*j+3*i] = IIC_Read_Byte(1);	
//			Data[9*j+3*i+1] = IIC_Read_Byte(1);		// ��ȡ�������ݣ��Ƿ���Ҫ����
//			if(j==j_max-1 && i==i_max-1) Data[9*j+3*i+2] = IIC_Read_Byte(0);
//			else Data[9*j+3*i+2] = IIC_Read_Byte(1);
//		}
//	}
	Data[0] = IIC_Read_Byte(1);	
	Data[1] = IIC_Read_Byte(1);	
	Data[2] = IIC_Read_Byte(1);		// ��ȡ�������ݣ��Ƿ���Ҫ����
	Data[3] = IIC_Read_Byte(1);
	Data[4] = IIC_Read_Byte(1);	
	Data[5] = IIC_Read_Byte(1);
	Data[6] = IIC_Read_Byte(1);
	Data[7] = IIC_Read_Byte(1);	
	Data[8] = IIC_Read_Byte(0);
	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
	/* ����I2C����ֹͣ�ź� */
	IIC_Stop();
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	IIC_Stop();
}
//max30102��ʼ��
void max30102_init()
{
	IIC_Init();
	max30102_reset();
	max30102_Bus_Write(REG_INTR_ENABLE_1,0x80);	// INTR setting---80
	max30102_Bus_Write(REG_INTR_ENABLE_2,0x00);
	max30102_Bus_Write(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
	max30102_Bus_Write(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
	max30102_Bus_Write(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
	max30102_Bus_Write(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17��P18
	max30102_Bus_Write(REG_MODE_CONFIG,0x03);  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	max30102_Bus_Write(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (411uS) ��P19 
	max30102_Bus_Write(REG_LED1_PA,0x44);   	//Choose value for ~ 7mA for LED1��P21
	max30102_Bus_Write(REG_LED2_PA,0x33);   	// Choose value for ~ 7mA for LED2
	//max30102_Bus_Write(REG_LED3_PA,0xff);   	// Choose value for ~ 7mA for LED3
	//max30102_Bus_Write(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED
	max30102_Bus_Write(REG_MULTI_LED_CTRL1,0x21);	// In multi-LED mode, which LED is active��P22
	max30102_Bus_Write(REG_MULTI_LED_CTRL2,0x03);	
// Finger: Red IR: 33
// Wrist up: Red 60, IR 40
// Wrist down: Red 80, IR 50; Red 55, IR 40
// 50,33 -> 37,36
}
//max30102��λ
void max30102_reset(void)
{
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
}
