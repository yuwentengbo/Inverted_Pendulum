#include "dc_moter.h"

void TIM1_PWM_Init(u16 arr,u16 psc);
void MY_GPIO_Init();

void dc_moter_init()
{
	TIM1_PWM_Init(899,0);
	MY_GPIO_Init();
}

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //??GPIO??????
	                                                                     	

   //????????????,??TIM1 CH1?PWM????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //???????????????????????????	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //??????TIMx???????????  ???
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_Pulse = 0; //????????????????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //????:TIM???????
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //??TIM_OCInitStruct???????????TIMx

    TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE ?????	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1?????	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //??TIMx?ARR????????
	
	//TIM_Cmd(TIM1, ENABLE);  //??TIM1

}


void MY_GPIO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2);					
}

/*
	last 500
*/
void dc_moter_setspeed(int speed)
{
	TIM_SetCompare1(TIM1,speed);
}