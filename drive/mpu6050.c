/****************************************Copyright (c)************************************************

** Descriptions:		mpu6050��������
**---------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf_drv_twi.h"
#include "mpu6050.h"



//TWI��������ʵ��ID,ID�������Ŷ�Ӧ��0:TWI0  1:TWI1
#define TWI_INSTANCE_ID     0

//TWI������ɱ�־
static volatile bool m_xfer_done = false;
//����TWI��������ʵ��������Ϊm_twi
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

//TWI�¼�������
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    //�ж�TWI�¼�����
	  switch (p_event->type)
    {
        //��������¼�
			  case NRF_DRV_TWI_EVT_DONE:
            m_xfer_done = true;//��λ������ɱ�־
            break;
        default:
            break;
    }
}
//TWI��ʼ��
void twi_master_init(void)
{
    ret_code_t err_code;
    //���岢��ʼ��TWI���ýṹ��
    const nrf_drv_twi_config_t twi_config = {
       .scl                = TWI_SCL_M,  //����TWI SCL����
       .sda                = TWI_SDA_M,  //����TWI SDA����
       .frequency          = NRF_DRV_TWI_FREQ_100K, //TWI����
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH, //TWI���ȼ�
       .clear_bus_init     = false//��ʼ���ڼ䲻����9��SCLʱ��
    };
    //��ʼ��TWI
    err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
	//��鷵�صĴ������
    APP_ERROR_CHECK(err_code);
    //ʹ��TWI
    nrf_drv_twi_enable(&m_twi);
}

/*************************************************************************
 * ��  �� : дMPU6050�Ĵ���
 * ��  �� : register_address[in]���Ĵ�����ַ
 *        : value[in]��д�������
 * ����ֵ : true:д���ݳɹ�,false��д��ʧ��
 *************************************************************************/ 
bool mpu6050_register_write(uint8_t register_address, uint8_t value)
{
	  ret_code_t err_code;
	  uint8_t tx_buf[MPU6050_ADDRESS_LEN+1];
	
	  //׼��д�������
		tx_buf[0] = register_address;
    tx_buf[1] = value;
	  //TWI������ɱ�־����Ϊfalse
		m_xfer_done = false;
		//д������
    err_code = nrf_drv_twi_tx(&m_twi, MPU6050_ADDRESS, tx_buf, MPU6050_ADDRESS_LEN+1, false);
	  //�ȴ�TWI���ߴ������
		while (m_xfer_done == false){}
	  if (NRF_SUCCESS != err_code)
    {
        return false;
    }
		return true;	
}
/*************************************************************************
 * ��  �� : ��MPU6050�Ĵ���
 * ��  �� : register_address[in]���Ĵ�����ַ
 *        : * destination[out]  ��ָ�򱣴��ȡ���ݵĻ���
 *        : number_of_bytes[in] ����ȡ�����ݳ���
 * ����ֵ : true:�����ɹ�,false������ʧ��
 *************************************************************************/ 
bool mpu6050_register_read(uint8_t register_address, uint8_t * destination, uint8_t number_of_bytes)
{
	  ret_code_t err_code;
	  //TWI������ɱ�־����Ϊfalse
		m_xfer_done = false;
	  err_code = nrf_drv_twi_tx(&m_twi, MPU6050_ADDRESS, &register_address, 1, true);
	  //�ȴ�TWI���ߴ������
		while (m_xfer_done == false){}
    if (NRF_SUCCESS != err_code)
    {
        return false;
    }
		//TWI������ɱ�־����Ϊfalse
		m_xfer_done = false;
	  err_code = nrf_drv_twi_rx(&m_twi, MPU6050_ADDRESS, destination, number_of_bytes);
		//�ȴ�TWI���ߴ������
		while (m_xfer_done == false){}
		if (NRF_SUCCESS != err_code)
    {
        return false;
    }
		return true;
}
/*************************************************************************
 * ��  �� : �����ٶ�ԭʼֵ
 * ��  �� : pACC_X[in]�����ٶ�x���ԭʼ���ݣ������ţ�
 *        : pACC_Y[in]�����ٶ�y���ԭʼ���ݣ������ţ�
 *        : pACC_Z[in]�����ٶ�z���ԭʼ���ݣ������ţ�
 * ����ֵ : true:��ȡ�ɹ���false����ȡʧ��
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
 * ��  �� : ��ʼ��MPU6050
 * ��  �� : ��
 * ����ֵ : true:��ʼ���ɹ���false����ʼ��ʧ��
 *************************************************************************/ 
bool mpu6050_init(void)
{   
  bool transfer_succeeded = true;
	
  //��֤MPU6050 ID
  transfer_succeeded &= mpu6050_verify_product_id();
	if(mpu6050_verify_product_id() == false)
	{
		return false;
	}

  //����MPU6050
	(void)mpu6050_register_write(MPU_PWR_MGMT1_REG , 0x00); 
  
	//����GYRO
	(void)mpu6050_register_write(MPU_SAMPLE_RATE_REG , 0x07); //���ò�����(Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV))��1KHz 
	(void)mpu6050_register_write(MPU_CFG_REG , 0x06); //���õ�ͨ�˲�������ֹƵ����1K��������5K							
	(void)mpu6050_register_write(MPU_INT_EN_REG, 0x00); //�ر��ж�	
  (void)mpu6050_register_write(MPU_GYRO_CFG_REG , 0x18); //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
  (void)mpu6050_register_write(MPU_ACCEL_CFG_REG,0x00); //���ü��ٶȴ��������� +-2G s�����Լ�  		

  return transfer_succeeded;
}
/*************************************************************************
 * ��  �� : ��������ԭʼֵ
 * ��  �� : pGYRO_X[in]��������x���ԭʼ���ݣ������ţ�
 *        : pGYRO_Y[in]��������y���ԭʼ���ݣ������ţ�
 *        : pGYRO_Z[in]��������z���ԭʼ���ݣ������ţ�
 * ����ֵ : true:��ȡ�ɹ���false����ȡʧ��
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
 * ��  �� : �����ٶ�ԭʼֵ
 * ��  �� : pACC_X[in]�����ٶ�x���ԭʼ���ݣ������ţ�
 *        : pACC_Y[in]�����ٶ�y���ԭʼ���ݣ������ţ�
 *        : pACC_Z[in]�����ٶ�z���ԭʼ���ݣ������ţ�
 * ����ֵ : true:��ȡ�ɹ���false����ȡʧ��
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





