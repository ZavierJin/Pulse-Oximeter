/* spo2_test.c
** Create by Jin Zhanyu (2020/10/9)
** Brief introduction: sensor 
*/


#include "spo2_test.h"
#include "ble_nus.h"


#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_uarte.h"
#include "nrf_log.h"
#include "driver_spi.h"
#include "nrf_drv_gpiote.h"
#include "max30102.h"
#include "myiic.h"
#include "lis2dh12.h"
#include <math.h>

#include "oled.h"

#define LED_TOTAL 		3			
#define BIOS			32	
#define BLE_MSG_LENGTH 	400	// Maximum length of message sent by Bluetooth
#define DATA_MAX_ID 	(1024+BIOS)		// Amount of data collected
//#define SAMPLE_MAX 100

volatile u8 COLLECT_FINISH = 0; // flag whether collection is finished 

uint32_t Collect_Data_Set[DATA_MAX_ID][LED_TOTAL];	// Store collected data
uint16_t Data_Set_Next_Id = 0;	// Next storage location index


uint32_t Data_Count = 0;	// For debug

#define G_100 98

uint16_t Acc_Limit_Up[2] = {11*G_100, 15*G_100};
uint16_t Acc_Limit_Down[2] = {9*G_100, 8*G_100};


static void send_data_to_app(void);
static void gpio_init(void);
static void get_sense_data(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
static void start_collection(void);


void spo2_test(void)
{
	u8 num = 0;
	u8 list[15] = {4,5,6,7,7,6,6,5,5,4,4,3,3,2,2};
	static uint16_t pre_draw_id = 0;
	
	
	// ------------ init max30105 ---------------
	APP_ERROR_CHECK(NRF_LOG_INIT());
    nrf_delay_ms(1000);
    NRF_LOG_PRINTF("Hello World!\n");
    max30102_init();
	LIS2DH12_init();
    nrf_delay_ms(1000);
    NRF_LOG_PRINTF("MAX30105 init!!\n");
    //get_sense_data(); //???
    nrf_delay_ms(1000);
    gpio_init();
    NRF_LOG_PRINTF("GPIO init!!!\n");
    nrf_delay_ms(1000);
    
	// Read once for opening interrupt
	start_collection();
	
	// ------------ init OLED ---------------
	NRF_LOG_PRINTF("Loading.....\n");
    //NRF_LOG_INIT();
	NRF_LOG_PRINTF("Hello World!\n");
	Initial_OLED();
	clear_screen();
	picture_1();
	update_num('|');
	NRF_LOG_PRINTF("OLED init!\n");
	
    while(1){
        if(COLLECT_FINISH){
            send_data_to_app(); // TODO: process data
			pre_draw_id = 0;
		}else{
	//		NRF_LOG_PRINTF("show picture\n");
	//		nrf_delay_ms(125);
			
//			if((pre_draw_id != 0 && Data_Set_Next_Id == 0) || 
//				(Data_Set_Next_Id / 10 > pre_draw_id))
//			{
//				draw_wave(list[num]);
//				num = (num+1) % 15;
//				pre_draw_id = Data_Set_Next_Id / 10;
//			}
		}
//		OLED_Print(0, 16, "abcd09876",TYPE16X16,TYPE8X16);
//		OLED_Print(16, 32, "efg12345",TYPE16X16,TYPE6X8);
//		NRF_LOG_PRINTF("show picture ok\n");
    }
    
}


static void send_data_to_app()
{
	uint32_t i;
    u8 ble_msg[BLE_MSG_LENGTH];     // int8 or int32 ??
	NRF_LOG_PRINTF("start sending!!!!!!!\n");
	u8 start_msg[] = "start";
	u8 end_msg[] = "end";
	
	my_ble_string_send(start_msg, strlen((char*)start_msg));
	nrf_delay_ms(20);
    for(i = BIOS; i < Data_Set_Next_Id; i++) {
		// display data in J-Link
        // NRF_LOG_PRINTF("%d,%d,%d\n", Collect_Data_Set[i][0], Collect_Data_Set[i][1], Collect_Data_Set[i][2]);
		
		// send data to app
		sprintf((char*)ble_msg, "%x,%x,%x", Collect_Data_Set[i][0], Collect_Data_Set[i][1], Collect_Data_Set[i][2]);
        my_ble_string_send(ble_msg, strlen((char*)ble_msg));
		
        nrf_delay_ms(20);             
    }
	my_ble_string_send(end_msg, strlen((char*)end_msg));
	// Send complete
	
	// Restart collection
    Data_Set_Next_Id = 0;	// Reset storage location index
    COLLECT_FINISH = 0;		// Reset collection flag
	nrf_drv_gpiote_in_event_enable(MAX_DRDY, true);
	start_collection();
}

static void gpio_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true); 
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(MAX_DRDY, &in_config, get_sense_data);
    APP_ERROR_CHECK(err_code);
    nrf_drv_gpiote_in_event_enable(MAX_DRDY, true);
}

static void get_sense_data(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    u8 j;
    u8 temp[9]; // contain original data 
	u8 temp_2[9]; // contain original data 
	uint32_t Motion_Data_Set[3];	// Store motion data, 0-X, 1-Y, 2-Z, 3-compose 
	uint32_t Motion_Syn = 0;
	//int32_t offset[] = {434, 678, 583};	// offset for axes acc??
	if(pin == MAX_DRDY && nrf_gpio_pin_read(MAX_DRDY) == 0){
		max30102_FIFO_ReadBytes(REG_FIFO_DATA, temp);		// Prevent FIFO overflow
		//Data_Count++;	// For debug
		
		
		
//		// ------------------------- for collect test data ------------------------------

		
		
		
		// ------------------------- for ble send ------------------------------
		if(!COLLECT_FINISH){
			if(Data_Set_Next_Id < DATA_MAX_ID) {
				// ------------------------- for collect ------------------------------
				for(j = 0; j < LED_TOTAL-1; j++){
					Collect_Data_Set[Data_Set_Next_Id][j] = (long)((long)(((long)temp[3*j] & 0x03) << 16) | ((long)temp[3*j+1] << 8) | ((long)temp[3*j+2]));
				}
				//NRF_LOG_PRINTF("MAX[%d](%d,%d)\n", Data_Count, Collect_Data_Set[Data_Set_Next_Id][0], Collect_Data_Set[Data_Set_Next_Id][1]);
				// get motion signal
				LIS2DH12_GetAccAxesRaw(temp_2);
				for(j = 0; j < 3; j++){
					Motion_Data_Set[j] = (int32_t)((int16_t)(((int16_t)temp_2[2*j+1]<<8) | (int16_t)temp_2[2*j])>>6)*8;
				}
				Motion_Syn = (int32_t)sqrt(Motion_Data_Set[0]*Motion_Data_Set[0]+Motion_Data_Set[1]*Motion_Data_Set[1]+Motion_Data_Set[2]*Motion_Data_Set[2]);
				//NRF_LOG_PRINTF("[%d]", Motion_Data_Set[3]);
				//NRF_LOG_PRINTF(" (%d, %d, %d)\n", Motion_Data_Set[0], Motion_Data_Set[1], Motion_Data_Set[2]); //
				Collect_Data_Set[Data_Set_Next_Id][2] = Motion_Syn;
				
				// Store collected data
				for(j = 0; j < LED_TOTAL-1; j++){
					Collect_Data_Set[Data_Set_Next_Id][j] = (long)((long)(((long)temp[3*j] & 0x03) << 16) | ((long)temp[3*j+1] << 8) | ((long)temp[3*j+2]));
				}
				//NRF_LOG_PRINTF("MAX[%d](%d,%d)\n", Data_Count, Collect_Data_Set[Data_Set_Next_Id][0], Collect_Data_Set[Data_Set_Next_Id][1]);
				// get motion signal
				LIS2DH12_GetAccAxesRaw(temp_2);
				for(j = 0; j < 3; j++){
					Motion_Data_Set[j] = (int32_t)((int16_t)(((int16_t)temp_2[2*j+1]<<8) | (int16_t)temp_2[2*j])>>6)*8;
				}
				Motion_Syn = (int32_t)sqrt(Motion_Data_Set[0]*Motion_Data_Set[0]+Motion_Data_Set[1]*Motion_Data_Set[1]+Motion_Data_Set[2]*Motion_Data_Set[2]);
				//NRF_LOG_PRINTF("[%d]", Motion_Data_Set[3]);
				//NRF_LOG_PRINTF(" (%d, %d, %d)\n", Motion_Data_Set[0], Motion_Data_Set[1], Motion_Data_Set[2]); //
				Collect_Data_Set[Data_Set_Next_Id][2] = Motion_Syn;
				NRF_LOG_PRINTF("%d,%d,%d\n", Collect_Data_Set[Data_Set_Next_Id][0], Collect_Data_Set[Data_Set_Next_Id][1], Collect_Data_Set[Data_Set_Next_Id][2]);
				Data_Set_Next_Id++;	// 标定数据时不增加
			}else {
				// Stop collection
				COLLECT_FINISH = 1;
				nrf_drv_gpiote_in_event_disable(MAX_DRDY);
			}
		}
	}
}

static void start_collection(void)
{
	u8 temp[9];
	
	// Read once for opening interrupt
	max30102_FIFO_ReadBytes(REG_FIFO_DATA, temp);	
	max30102_FIFO_ReadBytes(REG_FIFO_DATA, temp);	
	max30102_FIFO_ReadBytes(REG_FIFO_DATA, temp);	
	//LIS2DH12_FIFO_ReadBytes(REG_FIFO_DATA, temp);
	NRF_LOG_PRINTF("Start Collection!!!!\n");
}




