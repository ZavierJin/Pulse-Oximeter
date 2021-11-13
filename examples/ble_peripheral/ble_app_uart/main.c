/* main.c
** Version: 3.5
** Create by Jin Zhanyu (2020/5/23)
** Brief introduction: blood oxygen measurement by using MAX30105 sensor
** Pin connection instructions:
** MAX30105£º
**      SDA		<--->   IIC_SDA (P0.15) 
**      SCL		<--->   IIC_SCL (P0.16) 
**      I_M		<--->   MAX_DRDY (P0.31) 
**      I_L		<--->   LIS_DRDY (P0.30)  useless
**      VCC		<--->   3.3V (P9.8) up
**      GND		<--->   GND (P9.7)
** OLED£º
**      SDA		<--->   IIC_SDA (P0.25) 
**      SCL		<--->   IIC_SCL (P0.26) 
**      VCC		<--->   3.3V (P7.10)
**      GND		<--->   GND (P7.9)
** Modify:
**      2020/6/28 (by Jin Zhanyu):
**					use JLinkRTTViewer check output 
**      2020/7/23 :
**					1. add blue
**					2. send data successfully
**      2020/10/9 :
**					1. get 3 LED data simultaneously
**					2. reorganize files
**      2020/11/1 :
**					1. modify pins number
**      2020/12/9 :
**					1. fix ble bug, but cannot close interrupt
**      2020/12/13 :
**					1. read once first for opening interrupt
**					2. can close interrupt
**					3. modify interrupt pin number
**      2021/3/4 :
**					1. change to SpO2-Mode
**					2. add LIS2DH12 to get motion signal
**      2021/3/8 :
**					1. classification by movement
**      2021/4/4 :
**					1. add oled
**      2021/5/5 :
**					1. mix oled and max30105, change pins
**					2. send start & end message for ble
**      2021/5/19 :
**					1. Modify OLED
**					2. Modify sensor parameters
**					3. add ble recevie in ble_nus.c
**					4. fix restart bug
*/

#include "spo2_test.h"

int main(void)
{		
	my_ble_start();
   
	spo2_test();
		
	return 0;
}


