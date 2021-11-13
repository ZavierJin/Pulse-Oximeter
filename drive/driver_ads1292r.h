#ifndef _DRIVER_HAL_H_
#define _DRIVER_HAL_H_

#include "nrf_gpio.h"



#define ADS1292R_delaytime   10  //

//寄存器定义
//DEVICE SETTINGS
#define ADS1292R_ID			      (0x00)

//Global Settings
#define ADS1292R_CONFIG1			(0x01)
#define ADS1292R_CONFIG2			(0x02)
#define ADS1292R_LOFF			    (0x03)

//Channel-Specific Settings
#define ADS1292R_CH1SET			  (0x04)
#define ADS1292R_CH2SET			  (0x05)
#define ADS1292R_RLD_SENS		  (0x06)
#define ADS1292R_LOFFSENS		  (0x07)
#define ADS1292R_LOFF_STAT	  (0x08)

//GPIO and Other Registers
#define ADS1292R_RESP1			  (0x09)
#define ADS1292R_RESP2			  (0x0A)
#define ADS1292R_GPIO 			  (0x0B)

//控制命令定义
#define ADS1292R_CMD_WAKEUP			  (0x02)
#define ADS1292R_CMD_STANDBY			(0x04)
#define ADS1292R_CMD_RESET			  (0x06)
#define ADS1292R_CMD_START			  (0x08)
#define ADS1292R_CMD_STOP			    (0x0A)
#define ADS1292R_CMD_OFFSETCAL		(0x1A)

#define ADS1292R_CMD_RDATAC			  (0x10)
#define ADS1292R_CMD_SDATAC			  (0x11)
#define ADS1292R_CMD_RDATA			  (0x12)

#define ADS1292R_CMD_RREG 			  (0x20)
#define ADS1292R_CMD_WREG 			  (0x40)

//ADS1292R命令定义
//系统命令
	#define WAKEUP	0X02	//从待机模式唤醒
	#define STANDBY	0X04	//进入待机模式
	#define RESET	0X06	//复位ADS1292R
	#define START	0X08	//启动或转换
	#define STOP	0X0A	//停止转换
	#define OFFSETCAL	0X1A	//通道偏移校准
	
/////////////////////////////////////////////////////////////////////////////////////////////
//ID
#define	DEVICE_ID_ADS1292		0X53
#define	DEVICE_ID_ADS1292R	0X73
//CONFIG1
#define	DATA_RATE_125SPS	0X00	//采样率
#define	DATA_RATE_250SPS	0X01
#define	DATA_RATE_500SPS	0X02
#define	DATA_RATE_1kSPS		0X03
#define	DATA_RATE_2kSPS		0X04
#define	DATA_RATE_4kSPS		0X05
#define	DATA_RATE_8kSPS		0X06
//CONFIG2
#define	PDB_LOFF_COMP_OFF	0	//导联脱落比较器掉电（默认）
#define	PDB_LOFF_COMP_ON	1
#define	PDB_REFBUF_OFF	0		//基准电压缓冲器掉电（默认）
#define	PDB_REFBUF_ON		1
#define	VREF_242V		0	//内部参考电压2.42V（默认）
#define	VREF_4V			1	//内部参考电压4.033V
#define	CLK_EN_OFF	0		//振荡器时钟输出禁用（默认）
#define	CLK_EN_ON		1
#define	INT_TEST_OFF	0	//关闭内部测试信号（默认）
#define	INT_TEST_ON		1
//CHSET
#define	PD_ON			0	//通道正常运行（默认）	
#define	PD_OFF		1
#define	GAIN_6		0//增益6（默认）
#define	GAIN_1		1
#define	GAIN_2		2
#define	GAIN_3		3
#define	GAIN_4		4
#define	GAIN_8		5
#define	GAIN_12		6
#define	MUX_Normal_input 	0		//普通电极输入（默认）
#define	MUX_input_shorted 1		//输入短路
#define	MUX_Test_signal 	5		//测试信号输入
#define	MUX_RLD_DRP				6	
#define	MUX_RLD_DRM				7		
#define	MUX_RLD_DRPM			8	
#define	MUX_RSP_IN3P			9		//呼吸道通道1
//RLD_SENS
#define	PDB_RLD_OFF					0//（默认）
#define	PDB_RLD_ON					1
#define	RLD_LOFF_SENSE_OFF	0//（默认）
#define	RLD_LOFF_SENSE_ON		1
#define	RLD_CANNLE_OFF			0//（默认）
#define	RLD_CANNLE_ON				1
//LOFF_SENS
#define	FLIP2_OFF			0//（默认）
#define	FLIP2_ON			1							
#define	FLIP1_OFF			0//（默认）
#define	FLIP1_ON			1			
#define	LOFF_CANNLE_OFF		0//（默认）
#define	LOFF_CANNLE_ON		1	
//RSP1
#define	RESP_DEMOD_OFF	0	//（默认）
#define	RESP_DEMOD_ON		1	
#define	RESP_MOD_OFF 		0	//（默认）
#define	RESP_MOD_ON			1
#define	RESP_CTRL_CLOCK_INTERNAL	0
#define	RESP_CTRL_CLOCK_EXTERNAL	1
//RSP2
#define	CALIB_OFF		0//（默认）
#define	CALIB_ON		1		
#define	FREQ_32K		0		//（默认）
#define	FREQ_64K		1
#define	RLDREF_INT_EXTERN				0//外部馈电RLDREF
#define	RLDREF_INT_INTERNALLY		1//内部



//#define ads1292r_cs_high()         nrf_gpio_pin_set(SPI1_CS)
//#define ads1292r_cs_low()         nrf_gpio_pin_clear(SPI1_CS)

#define ads1292r_start_high()         nrf_gpio_pin_set(ADS_START)
#define ads1292r_start_low()         nrf_gpio_pin_clear(ADS_START)


//函数声明

void ads1292r_cs_low(void);
void ads1292r_cs_high(void);

void ads1292r_Init(void);
void ads1292r_ctrl_Init(void);
void ADS1292R_read_reg( uint8_t * p_buffer, uint8_t addr, uint8_t num );
void ADS1292R_write_reg( uint8_t * p_buffer, uint8_t addr, uint8_t num );
void ADS1292R_Set_RDATAC( void );
void ADS1292R_Set_SDATAC(void);
void ADS1292R_Reg_Init(void);
void ads1292r_read_data(uint8_t *data);
void ECGsensor_data_proc(void);
void ECG_dataPacking(void);	

#endif




