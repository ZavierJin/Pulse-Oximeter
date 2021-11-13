#include "driver_ads1292r.h"
#include "pca10040.h"
#include "nrf_delay.h"
#include "driver_spi.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_gpiote.h"

//extern RINGBUF8 t_ads_ch1_buf;     //ADSת�����ݶ�ȡ����
//extern SEND_STRUCT  send_packet_struct;
//extern RINGBUF8  ble_packet_struct;            //


void ads1292r_cs_low(void)
{
	SPI1_SCK_Init();
	 nrf_gpio_pin_clear(SPI1_CS);
}

void ads1292r_cs_high(void)
{
	 nrf_gpio_pin_set(SPI1_CS);
		SPI1_Uninit();
}

/*----------------------------------------------
1   ADS1922R ��ʼ��
--------------------------------------------------*/
void ads1292r_Init(void)
{	

//	SPI1_Init();	
	nrf_delay_ms(1000);

//  ads1292r_ctrl_Init();	
	nrf_delay_ms(1000);
//	ADS1292R_Set_SDATAC();
//	nrf_delay_ms(100);
//	ADS1292R_Reg_Init();
//		nrf_delay_ms(100);
//	ADS1292R_Set_RDATAC();
//	nrf_delay_ms(100);

		ads1292r_cs_low();
	spi1_readwrite_onebyte(0X08);    // ����ת��ģʽ
  ads1292r_start_high();                          // START�������
  nrf_delay_us(ADS1292R_delaytime);
nrf_delay_ms(100);

	
}

/*----------------------------------------------
2  ADS1922R�������ų�ʼ��
--------------------------------------------------*/
void ads1292r_ctrl_Init(void)
{
	nrf_gpio_cfg_output(ADS_PWDN);
	nrf_gpio_cfg_output(ADS_START);	
	nrf_gpio_cfg_output(SPI1_CS);	
	
	nrf_gpio_pin_set(ADS_PWDN);
	ads1292r_start_low();
	ads1292r_cs_high();
}

/*----------------------------------------------
3  ads1292R ���Ĵ���

p_buffer �����ȡ������������ָ��
addr     ����ʼ��ַ
num      : ��ȡ����
--------------------------------------------------*/
void ADS1292R_read_reg( uint8_t * p_buffer, uint8_t addr, uint8_t num )
{
		if(num<1) num=1;  //��ֹ����
	ads1292r_cs_low();
	
	spi1_readwrite_onebyte(addr|ADS1292R_CMD_RREG); //���Ĵ���+0x20
	spi1_readwrite_onebyte(num-1);
	while(num)
	{
		*p_buffer = spi1_readwrite_onebyte(0x00);
		num--;
		p_buffer++;
	}
	nrf_delay_us(ADS1292R_delaytime);
	ads1292r_cs_high();	
}

/*------------------------
4  ads1292R д�Ĵ���
����˵��
p_buffer�����ȡ������������ָ��
addr    ����ʼ��ַ
num     : ��ȡ����
-----------------------------------------*/
void ADS1292R_write_reg( uint8_t * p_buffer, uint8_t addr, uint8_t num )
{
	ads1292r_cs_low();
	if(num<1) num=1;  //��ֹ����
	
	spi1_readwrite_onebyte(addr+0x40); //���Ĵ���+0x40
	spi1_readwrite_onebyte(num-1);
	while(num)
	{
		*p_buffer = spi1_readwrite_onebyte(*p_buffer);
		num--;
		p_buffer++;
	}
	nrf_delay_us(ADS1292R_delaytime);
	ads1292r_cs_high();
	
}

/*-------------------------------

5  ����Ϊ����ת��ģʽ
---------------------------------------*/
void ADS1292R_Set_RDATAC( void )
{
	ads1292r_cs_low();
	spi1_readwrite_onebyte(ADS1292R_CMD_RDATAC);    // ����ת��ģʽ
  ads1292r_start_high();                          // START�������
  nrf_delay_us(ADS1292R_delaytime);
	ads1292r_cs_high();
}


/*--------------------------------------
6  �˳�����ת��ģʽ

-------------------------------------------*/
void ADS1292R_Set_SDATAC(void)
{
	int i=0;
	uint8_t regval[12];
	uint8_t startaddr=0;

ads1292r_cs_low();
// 	while(true){
//		
//		  nrf_delay_us(100);
//	    spi1_readwrite_onebyte(0xaa); //����mosi
//	   
//	}
	spi1_readwrite_onebyte(ADS1292R_CMD_SDATAC);  //�˳�����ת��ģʽ
	nrf_delay_us(ADS1292R_delaytime);
	ads1292r_start_low();	
	ads1292r_cs_high();
		
}


/*-------------------------------------------------

---------------------------------------------------------*/
void ADS1292R_Reg_Init(void)
{
uint8_t RegVal;
//	RegVal=0xA3;   //�����ź�
		RegVal=0xa0; //����ģʽ
	ADS1292R_write_reg( &RegVal,ADS1292R_CONFIG2, 1 );  //addr=02h, ���������źţ�1Hz����
  nrf_delay_ms(10);
	
	RegVal=0x01;
	ADS1292R_write_reg(&RegVal ,ADS1292R_CONFIG1 , 1 );  //addr=01h,������=250Hz  ֵ��ǰ����ַ�ں�

	RegVal=0x10;
	ADS1292R_write_reg( &RegVal,ADS1292R_LOFF, 1 );     //addr=03h

//	RegVal=0x05;	
	RegVal=0x00;
	ADS1292R_write_reg(&RegVal,ADS1292R_CH1SET, 1 );    // addr=04h,
	
//	RegVal=0x05;
	RegVal=0x00;
	ADS1292R_write_reg( &RegVal,ADS1292R_CH2SET, 1 );   //addr=05h
		
	RegVal=0x2C;
	ADS1292R_write_reg( &RegVal , ADS1292R_RLD_SENS, 1 );  ////addr=06h
	
	RegVal= 0x0E; // modify
	ADS1292R_write_reg( &RegVal ,ADS1292R_LOFFSENS, 1 );  //addr=07h
	
//		RegVal= 0x0F;
//	ADS1292R_write_reg( &RegVal ,ADS1292R_LOFF_STAT, 1 );  //addr=08h
	
		RegVal= 0x00;
	ADS1292R_write_reg( &RegVal ,ADS1292R_RESP1, 1 );  //addr=09h
	
	RegVal= 0x02;
	ADS1292R_write_reg( &RegVal ,ADS1292R_RESP2, 1 );  //addr=0Ah
	
	
		int i=0;
	uint8_t regval[12]={0};
	uint8_t startaddr=0;
		
	while(regval[0]!=0x73)
		{
     NRF_LOG_FLUSH();
	   ADS1292R_read_reg(regval,startaddr,12);
	   for(i=0;i<12;i++)
			{
			 NRF_LOG_INFO("Reg(%d):%x\r\n",i,regval[i]);
			}
	  nrf_delay_ms(100);
	}
	NRF_LOG_INFO("ADS1292R--succeed\r\n");
}



uint8_t ads_send_cnt=0;
uint8_t ads_offset=0;
uint8_t ads_raw_databuf[20];
/*-------------------------------------------------
��������ģʽ�¶�ȡת������
24 status bits + 24 bits �� 2 channels
the 24 status bits is: (1100 + LOFF_STAT[4:0] + GPIO[1:0] + 13 '0's).
-----------------------------------------------------*/
void ads1292r_read_data(uint8_t *data)
{
	uint8_t i;
	ads1292r_cs_low();

  uint8_t aa=0x00;
//	nrf_delay_us(ADS1292R_delaytime);

	for(i=0;i<9;i++)
	{
		*data = spi1_readwrite_onebyte(aa);
		data++;
	} 

//	nrf_delay_us(ADS1292R_delaytime);
	ads1292r_cs_high();

	
}

/*---------------------------------------------------------------

----------------------------------------------------------------*/
void ECGsensor_data_proc(void)
{	
//	if(RINGBUF8_GetBufLength(&t_ads_ch1_buf)>=60)
//	{
//    ECG_dataPacking();		
//	}
		
}

/*-----------------------------------------------------

--------------------------------------------------------*/
void ECG_dataPacking(void)
{
//	uint8_t result_data[160];    //������������
//	uint8_t i,j,blepacket_cnt;
//	
//	uint8_t DATA_LEN,CRC_H,CRC_L;
//	send_packet_struct.packet_serial++;
//	if(send_packet_struct.packet_serial>127) send_packet_struct.packet_serial=0;
//	
//	send_packet_struct.send_arry[0]=0xfa;
//	send_packet_struct.send_arry[1]=0xfa;
//	send_packet_struct.send_arry[2]=send_packet_struct.packet_serial;   //������
//	send_packet_struct.send_arry[3]=0x42;                              //������ԭʼ����
//	send_packet_struct.send_arry[4]=0x01;                              //�����룬���ݷ���
//	
//	result_data[0]=0x08;            // �������࣬ECG����
//	result_data[1]=0x01;            // �������ͣ�ADS1292Rͨ��1ԭʼ����
//	result_data[2]=0x00;            // �����ٶ� 125Hz
//	result_data[3]=0x00;            // �Ŵ���=6
//	
//	for(i=0;i<60;i++)
//	{
//		RINGBUF8_GetOneElement(&t_ads_ch1_buf,&result_data[i+4]);
////		result_data[i+4] =i;
//	}		
//	DATA_LEN=64;
//	crc16_get(result_data, DATA_LEN, &CRC_H, &CRC_L);      //������CRC16
//	result_data[64]=CRC_H;
//	result_data[65]=CRC_L;
//	
//	ARRAY_Insert_Mark(result_data,66, &send_packet_struct);  //��0����	
//  send_packet_struct.send_arry[3]=send_packet_struct.packet_length;//����������   
//	
//	DATA_LEN=send_packet_struct.send_arry[3]+5;      //�ܷ��ͳ���Ϊ(��������У���볤��ֵ) + (��ͷ�������С������Ⱥ������볤��ֵ)
//	//�����ְ���	
//	blepacket_cnt=DATA_LEN/19;
//	if((blepacket_cnt*19)<DATA_LEN)
//	{
//		blepacket_cnt++;
//	}
//	
//	//���
//	for(j=0;j<blepacket_cnt;j++)
//	{
//		RINGBUF8_Fill(&ble_packet_struct,j);  //��һ���ֽ�Ϊ0
//		for(i=0;i<19;i++)
//	 {
//		 RINGBUF8_Fill(&ble_packet_struct,send_packet_struct.send_arry[j*19+i]);
//	 }
//	}

}

