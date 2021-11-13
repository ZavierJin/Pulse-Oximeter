#include "lis2dh12.h"
#include "myiic.h"

u8 LIS2DH12_Bus_Write(u8 Register_Address, u8 Word_Data)
{
	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
	/* ��1��������I2C���������ź� */
	IIC_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(LIS2DH12_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��4���������ֽڵ�ַ */
	IIC_Send_Byte(Register_Address);	// LIS��һ���ĵط�������
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

u8 LIS2DH12_Bus_Read(u8 Register_Address)
{
	u8  data;
	/* ��1��������I2C���������ź� */
	IIC_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(LIS2DH12_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��4���������ֽڵ�ַ�� */
	IIC_Send_Byte(Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	IIC_Start();
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(LIS2DH12_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
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

//LIS2DH12��ʼ��
void LIS2DH12_init()
{	
	//IIC_Init();	//���ƶ�������max���Ѿ���ʼ��
	//LIS2DH12_reset();	//����
	LIS2DH12_Bus_Write(LIS_CTRL_REG1, 0x57); // 100Hz, �߷ֱ��ʣ�xyzʹ��
	//LIS2DH12_Bus_Write(LIS_CTRL_REG2, 0x57); // �ڲ��˲������ã����ᣩ
	//LIS2DH12_Bus_Write(LIS_CTRL_REG3, 0x02); // FIFO����ж�
	LIS2DH12_Bus_Write(LIS_CTRL_REG4, 0x10); // 4g������ģʽ10bit������1����4/512=7.8125mg
	//LIS2DH12_Bus_Write(LIS_CTRL_REG5, 0x40); // ����FIFO�Ƚ��ȳ�
	//LIS2DH12_Bus_Write(LIS_FIFO_CTRL_REG, 0xC0); // �����Ƚ��ȳ�ģʽ
}
//LIS2DH12��λ�����᣿��
//void LIS2DH12_reset(void)
//{
//	LIS2DH12_Bus_Write(LIS_MODE_CONFIG,0x40);
//	LIS2DH12_Bus_Write(LIS_MODE_CONFIG,0x40);
//}

//void LIS2DH12_FIFO_ReadBytes(u8 Register_Address, u8* Data)
//{	
////	LIS2DH12_Bus_Read(LIS_INTR_STATUS_1);	// ����жϱ�־����
////	LIS2DH12_Bus_Read(LIS_INTR_STATUS_2);	
//	/* ��1��������I2C���������ź� */
//	IIC_Start();
//	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	IIC_Send_Byte(LIS2DH12_WR_address | I2C_WR);	/* �˴���дָ�� */
//	/* ��3��������ACK */
//	if (IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}
//	/* ��4���������ֽڵ�ַ�� */
//	IIC_Send_Byte(Register_Address + 0x80);
//	if (IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}
//	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
//	IIC_Start();
//	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	IIC_Send_Byte(LIS2DH12_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
//	/* ��8��������ACK */
//	if (IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}
//	/* ��9������ȡ���� */
//	Data[0] = IIC_Read_Byte(1);	
//	Data[1] = IIC_Read_Byte(1);	
//	Data[2] = IIC_Read_Byte(1);		// ��ȡ�������ݣ��Ƿ���Ҫ����
//	Data[3] = IIC_Read_Byte(1);
//	Data[4] = IIC_Read_Byte(1);	
//	Data[5] = IIC_Read_Byte(1);
//	Data[6] = IIC_Read_Byte(0);
////	Data[7] = IIC_Read_Byte(1);	
////	Data[8] = IIC_Read_Byte(0);
//	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
//	/* ����I2C����ֹͣ�ź� */
//	IIC_Stop();
//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	IIC_Stop();
//}
