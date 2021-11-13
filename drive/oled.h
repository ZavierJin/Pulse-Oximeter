#include "stdint.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf52.h"


#ifndef	__OLED_H
#define	__OLED_H

#define  u8       uint8_t 
//#define  delay_us    nrf_delay_us 
//#define  delay_ms    nrf_delay_ms 

#define 	TYPE8X16		1
#define 	TYPE16X16		2
#define 	TYPE6X8			3

#define X_WIDTH 128
#define Y_WIDTH 64
#define	Brightness	0x7f

#define  OLED_SCL     26//4
#define  OLED_SDA     25//5

//IO方向设置
//#define SDA_IN()  {nrf_gpio_cfg_input(OLED_SDA,NRF_GPIO_PIN_NOPULL);}
//#define SDA_OUT() {nrf_gpio_cfg_output(OLED_SDA);}

//IO操作函数
#define OLED_SCL_high()         nrf_gpio_pin_set(OLED_SCL)
#define OLED_SCL_low()          nrf_gpio_pin_clear(OLED_SCL)

#define OLED_SDA_high()         nrf_gpio_pin_set(OLED_SDA)
#define OLED_SDA_low()          nrf_gpio_pin_clear(OLED_SDA)
#define READ_SDA               nrf_gpio_pin_read(OLED_SDA)  //输入SDA 

void Initial_OLED(void);
void picture_1(void);
void OLED_Start(void);
void OLED_Stop(void);
void Write_OLED_Command(unsigned char OLED_Command);
void Write_OLED_Data(unsigned char OLED_Data);
void Write_OLED_Byte(unsigned char OLED_Byte);
void OLED_Print(u8 x, u8 y, u8 *ch, u8 char_size, u8 ascii_size);
void clear_screen(void);
void OLED_Set_Pos(u8 x, u8 y);
void draw_icon(void);
void draw_tips(void);
void draw_number(u8 num);
void draw_wave(u8 scale);
void clear_number(void);
void update_num(u8 num);
void draw_big_number(u8 num, u8 pos);

#endif
