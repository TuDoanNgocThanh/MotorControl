#include "PWM.h"
#include "variable.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*
*/
void PWM_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/*GPIO PB8 & PB9*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ; 
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*Config Timer*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1; // Fclock = 1Mhz , T=1 uS
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 100- 1; // t = 1*100uS => f=10kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM4, ENABLE);
}
void PWM(int duty1,int duty2)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/*PWM1*/
	TIM_OCInitStructure.TIM_Pulse = duty1;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	/*PWM2*/
	TIM_OCInitStructure.TIM_Pulse = duty2;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}
double PID(void)
{
	Pre_Output=Output;
	Error_Current = Set_Speed-Current_Speed;

	Error_Sum = Error_Sum + Error_Current;
	Error_Derivative = Error_Current-Error_Past;

	Output = (Kp*Error_Current)+	(Ki*Error_Sum*T)	+	(Kd*Error_Derivative/T);
	Error_Past = Error_Current;
	if(Output >90)
		Output=89;
	if(Output<-90)
		Output=-89;
	if(abs(Output) <10)
		Output=0;
	if(abs(Error_Current)<2)
		Output=Pre_Output;
		return Output; //duty
}