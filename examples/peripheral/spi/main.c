/* main.h
** Version: 1.5
** Create by Jin Zhanyu (2020/5/23)
** Brief introduction: blood oxygen measurement by using MAX30105 sensor
** Pin connection instructions:
**      SDA		<--->   IIC_SDA (04)
**      SCL		<--->   IIC_SCL (05)
**      I_M		<--->   MAX_DRDY (15) 
**      VCC		<--->   3.3V (P7,10)
**      GND		<--->   GND (P7,9)
** Modify:
**      2020/6/28 (by Jin Zhanyu):
**			use JLinkRTTViewer check output 
*/



#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "driver_spi.h"
#include "nrf_drv_gpiote.h"
#include "max30102.h"
#include "myiic.h"
// Serial port
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif
//
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_nus.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "bsp_btn_ble.h"

#include "service.h"

//***************************************************

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */
#define BLE_STACK_SUPPORT_REQD 1

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}


#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED


u8 temp[9];	// contain original data 
volatile u8 max30102_recive_flag = 0;	// flag whether interrupt function completes data reading 

uint32_t sense_data[3];	// contain data for each LED
#define LED_TOTAL 3	

//#define SERIAL_OUTPUT 1
#define JLINK_OUTPUT 1

void get_sense_data(void);
void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);	// GPIOTE Interrupt
static void gpio_init(void);

void ble_test(void){
		uint32_t err_code;
    bool erase_bonds;

    // Initialize.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    uart_init();
    
    buttons_leds_init(&erase_bonds);
    ble_stack_init();
    gap_params_init();
    services_init();
    advertising_init();
    conn_params_init();

    printf("\r\nUART Start!\r\n");
    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
    
    // Enter main loop.
    for (;;)
    {
        power_manage();
    }
}

int main(void)
{
	ble_test();
/*	
#ifdef SERIAL_OUTPUT
	// Serial port 
	uint32_t err_code;
	const app_uart_comm_params_t comm_params =
		{
			RX_PIN_NUMBER,
			TX_PIN_NUMBER,
			RTS_PIN_NUMBER,
			CTS_PIN_NUMBER,
			UART_HWFC,
			false,
#if defined (UART_PRESENT)
			NRF_UART_BAUDRATE_115200
#else
			NRF_UARTE_BAUDRATE_115200
#endif
		};

	APP_UART_FIFO_INIT(&comm_params,
											 UART_RX_BUF_SIZE,
											 UART_TX_BUF_SIZE,
											 uart_error_handle,
											 APP_IRQ_PRIORITY_LOWEST,
											 err_code);

	APP_ERROR_CHECK(err_code);
#endif
	
	// main part
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();

	nrf_delay_ms(3000);
	NRF_LOG_RAW_INFO("\tOK!\r\n");
	//printf(" OK!\n");
	max30102_init();
	nrf_delay_ms(3000);
	NRF_LOG_RAW_INFO("\tMAX30105 init!!\r\n");
	//printf(" MAX30105 init!!\n");
	//get_sense_data();
	nrf_delay_ms(3000);
	gpio_init();
	NRF_LOG_RAW_INFO("\tGPIO init!!\r\n");
	//printf(" GPIO init!!\n");
	nrf_delay_ms(1000);
	
	while (1)
	{
#ifdef JLINK_OUTPUT
		NRF_LOG_FLUSH();	
#endif
		if (max30102_recive_flag == 1)
		{
			max30102_recive_flag = 0;
			//nrf_delay_ms(250);
			switch(LED_TOTAL){
				case 1: NRF_LOG_RAW_INFO("\t%d\r\n", sense_data[0]); break;
				case 2: NRF_LOG_RAW_INFO("\t%d, %d\r\n", sense_data[0], sense_data[1]); break;
				case 3: NRF_LOG_RAW_INFO("\t%d, %d, %d\r\n", sense_data[0], sense_data[1], sense_data[2]); break;
				default: NRF_LOG_RAW_INFO("\tLed is not turned on in the program!\r\n");
			}
			//printf("\t%d\t%d\n", sense_data[0], sense_data[1]);
			//printf(" No %04d %d\n", count_2, sense_data[0]);
			//printf(" No %04d %d %d %d\n", count_2, sense_data[0], sense_data[1], sense_data[2]);
		}
	}
*/
	
}
/********************************************END MAIN*******************************************/

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	if(pin == MAX_DRDY && nrf_gpio_pin_read(MAX_DRDY) == 0)
	{
		get_sense_data();
		max30102_recive_flag = 1;
	}
}

void get_sense_data(void)
{
	int i;
	max30102_FIFO_ReadBytes(REG_FIFO_DATA, temp);
		
	for(i=0; i<LED_TOTAL; ++i){
		sense_data[i] = (long)((long)((long)temp[3*i] & 0x03) << 16) | (long)temp[3*i+1] << 8 | (long)temp[3*i+2];
	}
}


static void gpio_init(void)
{
	ret_code_t err_code;

	err_code = nrf_drv_gpiote_init();
	APP_ERROR_CHECK(err_code);


	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true); 
	in_config.pull = NRF_GPIO_PIN_PULLUP;

	err_code = nrf_drv_gpiote_in_init(MAX_DRDY, &in_config, in_pin_handler);
	APP_ERROR_CHECK(err_code);
	nrf_drv_gpiote_in_event_enable(MAX_DRDY, true);
	
}







