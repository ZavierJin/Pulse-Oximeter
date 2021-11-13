#ifndef AT24C02_H__
#define AT24C02_H__
#include "nrf_delay.h"

//I2C引脚定义
#define TWI_SCL_M           22         //I2C SCL引脚
#define TWI_SDA_M           23         //I2C SDA引脚


#define MPU6050_ADDRESS_LEN  1         //MPU6050地址长度
#define MPU6050_ADDRESS     (0xD0>>1)  //MPU6050地址
#define MPU6050_WHO_AM_I     0x68U     //MPU6050 ID


#define MPU6050_GYRO_OUT        0x43
#define MPU6050_ACC_OUT         0x3B

#define ADDRESS_WHO_AM_I          (0x75U) // !< WHO_AM_I register identifies the device. Expected value is 0x68.
#define ADDRESS_SIGNAL_PATH_RESET (0x68U) // !<

//MPU6050寄存器
#define MPU_SELF_TESTX_REG		0x0D	//自检寄存器X
#define MPU_SELF_TESTY_REG		0x0E	//自检寄存器Y
#define MPU_SELF_TESTZ_REG		0x0F	//自检寄存器Z
#define MPU_SELF_TESTA_REG		0x10	//自检寄存器A
#define MPU_SAMPLE_RATE_REG		0x19	//采样频率分频器
#define MPU_CFG_REG				    0x1A	//配置寄存器
#define MPU_GYRO_CFG_REG		  0x1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		  0x1C	//加速度计配置寄存器
#define MPU_MOTION_DET_REG		0x1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG			  0x23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0x24	//IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG	0x25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG			  0x26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0x27	//IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG	0x28	//IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG			  0x29	//IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG	0x2A	//IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG	0x2B	//IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG			  0x2C	//IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG	0x2D	//IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG	0x2E	//IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG			  0x2F	//IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG	0x30	//IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG	0x31	//IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG			  0x32	//IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG		0x33	//IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG	0x34	//IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG		0x35	//IIC从机4读数据寄存器


#define MPU_PWR_MGMT1_REG		  0x6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		  0x6C	//电源管理寄存器2 

#define MPU_I2CMST_STA_REG		0x36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		  0x37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			  0x38	//中断使能寄存器
#define MPU_INT_STA_REG			  0x3A	//中断状态寄存器

#define MPU_I2CMST_DELAY_REG	0x67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0x68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0x69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		  0x6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		  0x6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		  0x6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG		  0x72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		  0x73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG			  0x74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG		  0x75	//器件ID寄存器

void twi_master_init(void);
bool mpu6050_init(void);

/**
  @brief Function for writing a MPU6050 register contents over TWI.
  @param[in]  register_address Register address to start writing to
  @param[in] value Value to write to register
  @retval true Register write succeeded
  @retval false Register write failed
*/
bool mpu6050_register_write(uint8_t register_address, const uint8_t value);

/**
  @brief Function for reading MPU6050 register contents over TWI.
  Reads one or more consecutive registers.
  @param[in]  register_address Register address to start reading from
  @param[in]  number_of_bytes Number of bytes to read
  @param[out] destination Pointer to a data buffer where read data will be stored
  @retval true Register read succeeded
  @retval false Register read failed
*/
bool mpu6050_register_read(uint8_t register_address, uint8_t *destination, uint8_t number_of_bytes);

/**
  @brief Function for reading and verifying MPU6050 product ID.
  @retval true Product ID is what was expected
  @retval false Product ID was not what was expected
*/
bool mpu6050_verify_product_id(void);


bool MPU6050_ReadGyro(int16_t *pGYRO_X , int16_t *pGYRO_Y , int16_t *pGYRO_Z );
bool MPU6050_ReadAcc( int16_t *pACC_X , int16_t *pACC_Y , int16_t *pACC_Z );

#endif


