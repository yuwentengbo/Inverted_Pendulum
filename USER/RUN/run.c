#include "run.h"
#include "usart.h"
#include "dc_moter.h"
#include "delay.h"
#include "encoder.h"
#include "math.h"

u8 left_run_flag,right_run_flag;
int left_time,right_time;
int left_step,right_step;
extern u8 tar;

/*

//MA = 0;
//MB = 1;			ni

//MA = 1;
//MB = 0;			shun


*/
void run1()
{
	dc_moter_setspeed(350);
	TIM_Cmd(TIM1, ENABLE);
	while(1)
	{
		MA = 0;
		MB = 1;
		delay_ms(500);
		MA = 1;
		MB = 0;
		delay_ms(500);
	}
	TIM_Cmd(TIM1, DISABLE);
	while(1);

}
void run2()
{
	dc_moter_setspeed(350);
	TIM_Cmd(TIM1, ENABLE);
	while(1)
	{
		MA = 0;
		MB = 1;
		delay_ms(500);
		MA = 1;
		MB = 0;
		delay_ms(500);
	}
	TIM_Cmd(TIM1, DISABLE);
	while(1);
}

extern u8 encoder_count;
void run3()
{
	u8 i;
	int pwm;
	float angle = 0.0,angle_now = 0.0,angle_dul = 0.0;
	
	for(i = 0;i < 20;i ++)
	{
		angle += encoder_get_angle();
	}
	angle = angle / 20.0;
	
	printf("angle %f\n",angle);
	while(1)		
	{
		angle_now = encoder_get_angle();
		if(encoder_count >= 2)
		{
			MA = 0;
			MB = 0;
			while(1);
		}
		angle_dul = angle_now - angle;		//jianxiao
		if(angle_dul < -3)
		{
			MA = 1;
			MB = 0;
		}
		else if(angle_dul > 3)
		{
			MA = 0;
			MB = 1;
		}
		else
		{
			
		}
		pwm = pid_balance(fabs(angle_dul));
		printf("pwm %d\n",pwm);
		dc_moter_setspeed(pwm);
		delay_ms(50);
	}
	
}


int pid_balance(float angle_dul)
{
	float kps = 10.0,kis = 0.0, kds = 0.0;		//p bili i jifen d weifen
	
	float pwm = 0.0;
	
	pwm = kps * angle_dul;
	
	if(pwm > 899)
	{
		pwm = 899;
	}
	return (899-(int)pwm);
}