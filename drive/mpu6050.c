/****************************************Copyright (c)************************************************

** Descriptions:		mpu6050驱动程序
**---------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf_drv_twi.h"
#include "mpu6050.h"



//TWI驱动程序实例ID,ID和外设编号对应，0:TWI0  1:TWI1
#define TWI_INSTANCE_ID     0

//TWI传输完成标志
static volatile bool m_xfer_done = false;
//定义TWI驱动程序实例，名称为m_twi
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

//TWI事件处理函数
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    //判断TWI事件类型
	  switch (p_event->type)
    {
        //传输完成事件
			  case NRF_DRV_TWI_EVT_DONE:
            m_xfer_done = true;//置位传输完成标志
            break;
        default:
            break;
    }
}
//TWI初始化
void twi_master_init(void)
{
    ret_code_t err_code;
    //定义并初始化TWI配置结构体
    const nrf_drv_twi_config_t twi_config = {
       .scl                = TWI_SCL_M,  //定义TWI SCL引脚
       .sda                = TWI_SDA_M,  //定义TWI SDA引脚
       .frequency          = NRF_DRV_TWI_FREQ_100K, //TWI速率
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH, //TWI优先级
       .clear_bus_init     = false//初始化期间不发送9个SCL时钟
    };
    //初始化TWI
    err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
	//检查返回的错误代码
    APP_ERROR_CHECK(err_code);
    //使能TWI
    nrf_drv_twi_enable(&m_twi);
}

/*************************************************************************
 * 功  能 : 写MPU6050寄存器
 * 参  数 : register_address[in]：寄存器地址
 *        : value[in]：写入的数据
 * 返回值 : true:写数据成功,false：写入失败
 *************************************************************************/ 
bool mpu6050_register_write(uint8_t register_address, uint8_t value)
{
	  ret_code_t err_code;
	  uint8_t tx_buf[MPU6050_ADDRESS_LEN+1];
	
	  //准备写入的数据
		tx_buf[0] = register_address;
    tx_buf[1] = value;
	  //TWI传输完成标志设置为false
		m_xfer_done = false;
		//写入数据
    err_code = nrf_drv_twi_tx(&m_twi, MPU6050_ADDRESS, tx_buf, MPU6050_ADDRESS_LEN+1, false);
	  //等待TWI总线传输完成
		while (m_xfer_done == false){}
	  if (NRF_SUCCESS != err_code)
    {
        return false;
    }
		return true;	
}
/*************************************************************************
 * 功  能 : 读MPU6050寄存器
 * 参  数 : register_address[in]：寄存器地址
 *        : * destination[out]  ：指向保存读取数据的缓存
 *        : number_of_bytes[in] ：读取的数据长度
 * 返回值 : true:操作成功,false：操作失败
 *************************************************************************/ 
bool mpu6050_register_read(uint8_t register_address, uint8_t * destination, uint8_t number_of_bytes)
{
	  ret_code_t err_code;
	  //TWI传输完成标志设置为false
		m_xfer_done = false;
	  err_code = nrf_drv_twi_tx(&m_twi, MPU6050_ADDRESS, &register_address, 1, true);
	  //等待TWI总线传输完成
		while (m_xfer_done == false){}
    if (NRF_SUCCESS != err_code)
    {
        return false;
    }
		//TWI传输完成标志设置为false
		m_xfer_done = false;
	  err_code = nrf_drv_twi_rx(&m_twi, MPU6050_ADDRESS, destination, number_of_bytes);
		//等待TWI总线传输完成
		while (m_xfer_done == false){}
		if (NRF_SUCCESS != err_code)
    {
        return false;
    }
		return true;
}
/*************************************************************************
 * 功  能 : 读加速度原始值
 * 参  数 : pACC_X[in]：加速度x轴的原始数据（带符号）
 *        : pACC_Y[in]：加速度y轴的原始数据（带符号）
 *        : pACC_Z[in]：加速度z轴的原始数据（带符号）
 * 返回值 : true:读取成功，false：读取失败
 *************************************************************************/ 
bool mpu6050_verify_product_id(void)
{
    uint8_t who_am_i;

    if (mpu6050_register_read(ADDRESS_WHO_AM_I, &who_am_i, 1))
    {
        if (who_am_i != MPU6050_WHO_AM_I)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}
/*************************************************************************
 * 功  能 : 初始化MPU6050
 * 参  数 : 无
 * 返回值 : true:初始化成功，false：初始化失败
 *************************************************************************/ 
bool mpu6050_init(void)
{   
  bool transfer_succeeded = true;
	
  //验证MPU6050 ID
  transfer_succeeded &= mpu6050_verify_product_id();
	if(mpu6050_verify_product_id() == false)
	{
		return false;
	}

  //唤醒MPU6050
	(void)mpu6050_register_write(MPU_PWR_MGMT1_REG , 0x00); 
  
	//设置GYRO
	(void)mpu6050_register_write(MPU_SAMPLE_RATE_REG , 0x07); //设置采样率(Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV))：1KHz 
	(void)mpu6050_register_write(MPU_CFG_REG , 0x06); //设置低通滤波器，截止频率是1K，带宽是5K							
	(void)mpu6050_register_write(MPU_INT_EN_REG, 0x00); //关闭中断	
  (void)mpu6050_register_write(MPU_GYRO_CFG_REG , 0x18); //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
  (void)mpu6050_register_write(MPU_ACCEL_CFG_REG,0x00); //配置加速度传感器量程 +-2G s，不自检  		

  return transfer_succeeded;
}
/*************************************************************************
 * 功  能 : 读陀螺仪原始值
 * 参  数 : pGYRO_X[in]：陀螺仪x轴的原始数据（带符号）
 *        : pGYRO_Y[in]：陀螺仪y轴的原始数据（带符号）
 *        : pGYRO_Z[in]：陀螺仪z轴的原始数据（带符号）
 * 返回值 : true:读取成功，false：读取失败
 *************************************************************************/ 
bool MPU6050_ReadGyro(int16_t *pGYRO_X , int16_t *pGYRO_Y , int16_t *pGYRO_Z )
{
	uint8_t buf[6]; 
  bool ret = false;	
	
	if(mpu6050_register_read(MPU6050_GYRO_OUT,  buf, 6) == true)
	{
		*pGYRO_X = (buf[0] << 8) | buf[1];
	  if(*pGYRO_X & 0x8000) *pGYRO_X-=65536;
		
	  *pGYRO_Y= (buf[2] << 8) | buf[3];
    if(*pGYRO_Y & 0x8000) *pGYRO_Y-=65536;
	
    *pGYRO_Z = (buf[4] << 8) | buf[5];
	  if(*pGYRO_Z & 0x8000) *pGYRO_Z-=65536;
		
		ret = true;
	}

	return ret;
}		   
/*************************************************************************
 * 功  能 : 读加速度原始值
 * 参  数 : pACC_X[in]：加速度x轴的原始数据（带符号）
 *        : pACC_Y[in]：加速度y轴的原始数据（带符号）
 *        : pACC_Z[in]：加速度z轴的原始数据（带符号）
 * 返回值 : true:读取成功，false：读取失败
 *************************************************************************/ 
bool MPU6050_ReadAcc( int16_t *pACC_X , int16_t *pACC_Y , int16_t *pACC_Z )
{
	uint8_t buf[6];
  bool ret = false;		
  if(mpu6050_register_read(MPU6050_ACC_OUT, buf, 6) == true)
	{
		mpu6050_register_read(MPU6050_ACC_OUT, buf, 6);
		*pACC_X = (buf[0] << 8) | buf[1];
		if(*pACC_X & 0x8000) *pACC_X-=65536;

		*pACC_Y= (buf[2] << 8) | buf[3];
		if(*pACC_Y & 0x8000) *pACC_Y-=65536;

		*pACC_Z = (buf[4] << 8) | buf[5];
		if(*pACC_Z & 0x8000) *pACC_Z-=65536;
		ret = true;
	}
	return ret;
}





