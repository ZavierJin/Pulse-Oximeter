#include "nrf52.h"
#include "nrf_gpio.h"
#include "exit.h"


 void Delay(uint32_t temp)
{
  for(; temp!= 0; temp--);
} 

void EXIT_KEY_Init(void)
{

	 nrf_gpio_cfg_input(KEY_0,NRF_GPIO_PIN_PULLUP);//设置管脚位上拉输入
	 nrf_gpio_cfg_input(KEY_1,NRF_GPIO_PIN_PULLUP);//设置管脚位上拉输入
    
    NVIC_EnableIRQ(GPIOTE_IRQn);//中断嵌套设置
	
    NRF_GPIOTE->CONFIG[0] =  (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)
                           | (13 << GPIOTE_CONFIG_PSEL_Pos)  
                           | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);//中断配置（详细说明请参看青风教程）
	 
    NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;// 使能中断类型:
	  NRF_GPIOTE->CONFIG[1] =  (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)
                           | (14<< GPIOTE_CONFIG_PSEL_Pos)  
                           | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);//中断配置（详细说明请参看青风教程）
	
    NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN1_Set << GPIOTE_INTENSET_IN1_Pos;// 使能中断类型:
}


void GPIOTE_IRQHandler(void)
{

    if ((NRF_GPIOTE->EVENTS_IN[0] == 1) && 
        (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN0_Msk))
    {
        NRF_GPIOTE->EVENTS_IN[0] = 0; //中断事件清零.
			  Delay(10000);	
			 if(nrf_gpio_pin_read(KEY_0)== 0)
			 {
			 
			 }
			
    }

		 if ((NRF_GPIOTE->EVENTS_IN[1] == 1) && 
        (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN1_Msk))
    {
        NRF_GPIOTE->EVENTS_IN[1] = 0; //中断事件清零.
			 
			
    }

 
}
