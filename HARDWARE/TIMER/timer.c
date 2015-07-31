#include "timer.h"
#include "delay.h"
#include "usart.h"
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	//TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
					 
}

u8 tar = 0;
extern u8 left_run_flag;
extern int left_time;
extern int left_step;
int left_time_now = 0;
int left_x = 0;


//left irq
void TIM3_IRQHandler(void)   //TIM3中断
{
	static int step =0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  //清除TIMx的中断待处理位:TIM 中断源
		
		left_time_now++;

		//printf("hello world\n");
		if(left_time_now == left_time)			//bujin left run
		{
			printf("left ::\n");
			left_time_now = 0;
			if(left_run_flag == 0)				
			{
				//Bujin_Left_Go(left_x);
				left_x++;
				left_x = left_x % 8;
			}
			else if(left_run_flag == 1)        
			{
				left_x--;
				if(left_x == -1)	left_x = 7;
				//Bujin_Left_Go(left_x);
			}
			step ++;
			if(step == left_step)
			{
				printf("left 123::\n");
				step = 0;
				tar ++;
				TIM_Cmd(TIM3,DISABLE);
			}
		}
	}
}

extern u8 right_run_flag;
extern int right_time;
extern int right_step;
int right_time_now = 0;
int right_x = 0;


void TIM2_IRQHandler(void)   //TIM2中断  right bujin
{
	static int step = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
		
		right_time_now++;

		if(right_time_now == right_time)
		{
			printf("right ::\n");
			right_time_now = 0;
			if(right_run_flag == 0)
			{
				//Bujin_Right_Go(right_x);
				right_x ++;
				right_x = right_x % 8;
			}
			else if(right_run_flag == 0)
			{
				//Bujin_Right_Go(right_x);
				right_x --;
				if(right_x == -1)		right_x = 7;
			}
			step ++;
			if(step == right_step)
			{
				step = 0;
				tar ++;
				printf("right 123 ::\n");
				TIM_Cmd(TIM2,DISABLE);
			}
		}
		//printf("hello world  qqq\n");
	}
}








