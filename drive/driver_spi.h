#ifndef __DRIVER_SPI_H__
#define __DRIVER_SPI_H__


#include "nrf52.h"

#define ADS_START          2
#define ADS_PWDN           3

#define SPI1_CS          11
#define SPI1_MOSI        15
#define SPI1_SCK         13
#define SPI1_MISO        14


#define ADS_DRDY           17	// modify


//SPI1
void SPI1_Init(void);
void SPI1_Uninit(void);
void SPI1_SCK_Init(void);
uint8_t spi1_readwrite_onebyte(uint8_t byte);











#endif
