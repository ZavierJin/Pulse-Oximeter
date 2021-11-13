#include "driver_spi.h"
#include "nrf_gpio.h"
#include "pca10040.h"
#include "nrf_drv_spi.h"

//static const nrf_drv_spi_t  LIS3DSH_spi = NRF_DRV_SPI_INSTANCE(0);  /**< SPI instance. */


//static const nrf_drv_spi_t  ads_spi     = NRF_DRV_SPI_INSTANCE(1);  /**< SPI instance. */





/*-----------------------------------------------------------------
SPI1�ӿڳ�ʼ��
--------------------------------------------------------------------*/
void SPI1_Init(void)
{
	  NRF_P0->PIN_CNF[SPI1_CS]   = 0x0D;       //0x01���ģʽ
		NRF_P0->PIN_CNF[SPI1_SCK]  = 0x05;        //05   0x0d??
		NRF_P0->PIN_CNF[SPI1_MOSI] = 0x05;       //���ģʽ,disconnect input buffer
		NRF_P0->PIN_CNF[SPI1_MISO] = 0x06;       //����ģʽ,disconnect input buffer
		NRF_P0->OUTSET = (1UL << SPI1_SCK);	 
	
	  NRF_SPI1->PSEL.SCK = SPI1_SCK;         //���õ�ָ������
		NRF_SPI1->PSEL.MOSI = SPI1_MOSI;
		NRF_SPI1->PSEL.MISO = SPI1_MISO;
			
		NRF_SPI1->FREQUENCY =0x08000000;     //0x02000000:125kbps, 0x08000000��500kbps  0x10000000:1Mbps
		NRF_SPI1->CONFIG = 0x00000004;
		
		NRF_SPI1->EVENTS_READY = 0U;
		NRF_SPI1->ENABLE = 0x00000001;        //ʹ��SPI1
}


void SPI1_Uninit(void)
{
		NRF_SPI1->CONFIG = 0x00000000;
}

void SPI1_SCK_Init(void)
{
	NRF_SPI1->CONFIG = 0x00000004;
}
/*-----------------------------------------------------
SPI0�˿ڶ�дһ���ֽ�����
--------------------------------------------------------*/
uint8_t spi0_readwrite_onebyte(uint8_t byte)
{
	uint8_t temp = 0;
	
	NRF_SPI0->TXD = byte;
	NRF_SPI0->ENABLE = 0x01;
	while(!(NRF_SPI0->EVENTS_READY))
	{}
	temp = NRF_SPI0->RXD;
	NRF_SPI0->EVENTS_READY = 0x00;
	return temp;
}

//	uint8_t sendval;
/*-------------------------------------------------------
SPI1�˿ڶ�дһ�ֽ�����
//----------------------------------------------------------*/
uint8_t spi1_readwrite_onebyte(uint8_t byte)
{
//  sendval=byte;
//	uint8_t temp = 0;
//	nrf_drv_spi_transfer(&ads_spi,&sendval,1,&temp,1);
	
	uint8_t temp = 0;
	NRF_SPI1->TXD = byte;
	NRF_SPI1->ENABLE = 0x01;
	while(!(NRF_SPI1->EVENTS_READY))
	{}
	temp = NRF_SPI1->RXD;
	NRF_SPI1->EVENTS_READY = 0x00;
	
	return temp;
}



