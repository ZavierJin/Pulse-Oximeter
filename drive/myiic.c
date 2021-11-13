#include "myiic.h"
#include "nrf_gpio.h"

//IO��������
#define SDA_IN()  {nrf_gpio_cfg_input(IIC_SDA,NRF_GPIO_PIN_NOPULL);}
#define SDA_OUT() {nrf_gpio_cfg_output(IIC_SDA);}

//IO��������
#define IIC_SCL_high()         nrf_gpio_pin_set(IIC_SCL)
#define IIC_SCL_low()          nrf_gpio_pin_clear(IIC_SCL)

#define IIC_SDA_high()         nrf_gpio_pin_set(IIC_SDA)
#define IIC_SDA_low()          nrf_gpio_pin_clear(IIC_SDA)
#define READ_SDA               nrf_gpio_pin_read(IIC_SDA)  //����SDA 


//��ʼ��IIC
void IIC_Init(void)
{
    //IO������
	//P16---> SCL��P17 ---> SDA
	  nrf_gpio_cfg_output(IIC_SCL);
	  nrf_gpio_cfg_output(IIC_SDA);

    IIC_SCL_high();
    IIC_SDA_high();
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
    SDA_OUT();     //SDA�����
    IIC_SDA_high();
    IIC_SCL_high();
    delay_us(4);
    IIC_SDA_low(); //START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC_SCL_low(); //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
    SDA_OUT();//sda�����
    IIC_SCL_low();
    IIC_SDA_low(); //STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL_high();
    IIC_SDA_high(); //����I2C���߽����ź�
    delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN();      //SDA����Ϊ����
    IIC_SDA_high();
    delay_us(1);
    IIC_SCL_high();
    delay_us(1);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_low(); //ʱ�����0
    return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
    IIC_SCL_low();
    SDA_OUT();
    IIC_SDA_low();
    delay_us(2);
    IIC_SCL_high();
    delay_us(2);
    IIC_SCL_low();
}
//������ACKӦ��
void IIC_NAck(void)
{
    IIC_SCL_low();
    SDA_OUT();
    IIC_SDA_high();
    delay_us(2);
    IIC_SCL_high();
    delay_us(2);
    IIC_SCL_low();
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL_low(); //����ʱ�ӿ�ʼ���ݴ���
    for(t = 0; t < 8; t++)
    {
			  if((txd & 0x80) >> 7) IIC_SDA_high();
				else        IIC_SDA_low();
        txd <<= 1;
        delay_us(2);   //��TEA5767��������ʱ���Ǳ����
        IIC_SCL_high();
        delay_us(2);
        IIC_SCL_low();
        delay_us(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();//SDA����Ϊ����
    for(i = 0; i < 8; i++ )
    {
        IIC_SCL_low();
        delay_us(2);
        IIC_SCL_high();
        receive <<= 1;
        if(READ_SDA)receive++;
        delay_us(1);
    }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}

void IIC_WriteBytes(u8 WriteAddr, u8* data, u8 dataLength)
{
    u8 i;
    IIC_Start();

    IIC_Send_Byte(WriteAddr);	    //����д����
    IIC_Wait_Ack();

    for(i = 0; i < dataLength; i++)
    {
        IIC_Send_Byte(data[i]);
        IIC_Wait_Ack();
    }
    IIC_Stop();//����һ��ֹͣ����
    delay_ms(10);
}

void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr, u8* data, u8 dataLength)
{
    u8 i;
    IIC_Start();

    IIC_Send_Byte(deviceAddr);	    //����д����
    IIC_Wait_Ack();
    IIC_Send_Byte(writeAddr);
    IIC_Wait_Ack();
    IIC_Send_Byte(deviceAddr | 0X01); //�������ģʽ
    IIC_Wait_Ack();

    for(i = 0; i < dataLength - 1; i++)
    {
        data[i] = IIC_Read_Byte(1);
    }
    data[dataLength - 1] = IIC_Read_Byte(0);
    IIC_Stop();//����һ��ֹͣ����
    delay_ms(10);
}




