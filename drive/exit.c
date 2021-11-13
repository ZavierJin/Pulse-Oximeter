#include "nrf52.h"
#include "nrf_gpio.h"
#include "exit.h"


 void Delay(uint32_t temp)
{
  for(; temp!= 0; temp--);
} 

void EXIT_KEY_Init(void)
{

	 nrf_gpio_cfg_input(KEY_0,NRF_GPIO_PIN_PULLUP);//���ùܽ�λ��������
	 nrf_gpio_cfg_input(KEY_1,NRF_GPIO_PIN_PULLUP);//���ùܽ�λ��������
    
    NVIC_EnableIRQ(GPIOTE_IRQn);//�ж�Ƕ������
	
    NRF_GPIOTE->CONFIG[0] =  (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)
                           | (13 << GPIOTE_CONFIG_PSEL_Pos)  
                           | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);//�ж����ã���ϸ˵����ο����̳̣�
	 
    NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;// ʹ���ж�����:
	  NRF_GPIOTE->CONFIG[1] =  (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)
                           | (14<< GPIOTE_CONFIG_PSEL_Pos)  
                           | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);//�ж����ã���ϸ˵����ο����̳̣�
	
    NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN1_Set << GPIOTE_INTENSET_IN1_Pos;// ʹ���ж�����:
}


void GPIOTE_IRQHandler(void)
{

    if ((NRF_GPIOTE->EVENTS_IN[0] == 1) && 
        (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN0_Msk))
    {
        NRF_GPIOTE->EVENTS_IN[0] = 0; //�ж��¼�����.
			  Delay(10000);	
			 if(nrf_gpio_pin_read(KEY_0)== 0)
			 {
			 
			 }
			
    }

		 if ((NRF_GPIOTE->EVENTS_IN[1] == 1) && 
        (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN1_Msk))
    {
        NRF_GPIOTE->EVENTS_IN[1] = 0; //�ж��¼�����.
			 
			
    }

 
}
