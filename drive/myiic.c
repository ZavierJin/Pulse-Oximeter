#include "myiic.h"
#include "nrf_gpio.h"

//IO方向设置
#define SDA_IN()  {nrf_gpio_cfg_input(IIC_SDA,NRF_GPIO_PIN_NOPULL);}
#define SDA_OUT() {nrf_gpio_cfg_output(IIC_SDA);}

//IO操作函数
#define IIC_SCL_high()         nrf_gpio_pin_set(IIC_SCL)
#define IIC_SCL_low()          nrf_gpio_pin_clear(IIC_SCL)

#define IIC_SDA_high()         nrf_gpio_pin_set(IIC_SDA)
#define IIC_SDA_low()          nrf_gpio_pin_clear(IIC_SDA)
#define READ_SDA               nrf_gpio_pin_read(IIC_SDA)  //输入SDA 


//初始化IIC
void IIC_Init(void)
{
    //IO口配置
	//P16---> SCL，P17 ---> SDA
	  nrf_gpio_cfg_output(IIC_SCL);
	  nrf_gpio_cfg_output(IIC_SDA);

    IIC_SCL_high();
    IIC_SDA_high();
}
//产生IIC起始信号
void IIC_Start(void)
{
    SDA_OUT();     //SDA线输出
    IIC_SDA_high();
    IIC_SCL_high();
    delay_us(4);
    IIC_SDA_low(); //START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC_SCL_low(); //钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    IIC_SCL_low();
    IIC_SDA_low(); //STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL_high();
    IIC_SDA_high(); //发送I2C总线结束信号
    delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN();      //SDA设置为输入
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
    IIC_SCL_low(); //时钟输出0
    return 0;
}
//产生ACK应答
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
//不产生ACK应答
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL_low(); //拉低时钟开始数据传输
    for(t = 0; t < 8; t++)
    {
			  if((txd & 0x80) >> 7) IIC_SDA_high();
				else        IIC_SDA_low();
        txd <<= 1;
        delay_us(2);   //对TEA5767这三个延时都是必须的
        IIC_SCL_high();
        delay_us(2);
        IIC_SCL_low();
        delay_us(2);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}

void IIC_WriteBytes(u8 WriteAddr, u8* data, u8 dataLength)
{
    u8 i;
    IIC_Start();

    IIC_Send_Byte(WriteAddr);	    //发送写命令
    IIC_Wait_Ack();

    for(i = 0; i < dataLength; i++)
    {
        IIC_Send_Byte(data[i]);
        IIC_Wait_Ack();
    }
    IIC_Stop();//产生一个停止条件
    delay_ms(10);
}

void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr, u8* data, u8 dataLength)
{
    u8 i;
    IIC_Start();

    IIC_Send_Byte(deviceAddr);	    //发送写命令
    IIC_Wait_Ack();
    IIC_Send_Byte(writeAddr);
    IIC_Wait_Ack();
    IIC_Send_Byte(deviceAddr | 0X01); //进入接收模式
    IIC_Wait_Ack();

    for(i = 0; i < dataLength - 1; i++)
    {
        data[i] = IIC_Read_Byte(1);
    }
    data[dataLength - 1] = IIC_Read_Byte(0);
    IIC_Stop();//产生一个停止条件
    delay_ms(10);
}




