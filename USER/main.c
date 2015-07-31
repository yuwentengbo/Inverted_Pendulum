#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "key.h"
#include "run.h"
#include "encoder.h"
#include "dc_moter.h"


void System_init()
{
	NVIC_Configuration();
	delay_init();
	KEY_Init();
	uart_init(9600);
	dc_moter_init();
	encoder_init();
}
extern u8 encoder_count;
int main(void)
{	
	u8 key_value = 0;
	int i =0;
	float encoder_angle = 0.0;
	
	System_init();
	
	/*while(1)
	{
		encoder_angle = encoder_get_angle();
		printf("%f\n",encoder_angle);
		//USART1->DR = '0';
		if(encoder_count == 2)
		{
			while(1);
		}
		delay_ms(50);
	}*/
	while(1)
	{
		while(key_value == 0)
		{
			key_value = KEY_Scan(0);
			delay_ms(10);
		}
		switch(key_value)
		{
			case KEY0_PRES:		run1();		break;
			case KEY1_PRES:		run2();     break;
			case WKUP_PRES:		run3();     break;
			default: 						break;
		}
	}
	
	return 0;
}