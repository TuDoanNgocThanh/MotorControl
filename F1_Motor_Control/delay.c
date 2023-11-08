#include "delay.h"
#include "variable.h"

/*delay ms*/
void delay_ms(uint16_t a)
{
	ms=a;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 48000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision =0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_SetCounter(TIM1,0);
	TIM_Cmd(TIM1, ENABLE);
	
	while(TIM_GetCounter(TIM1)<ms);
	TIM_Cmd(TIM1,DISABLE);
}

/*delay us*/
void delay_us(uint16_t b)
{
	us=b;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1;
	TIM_TimeBaseStructure.TIM_ClockDivision =0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_SetCounter(TIM1,0);
	TIM_Cmd(TIM1,ENABLE);
	
	while(TIM_GetCounter(TIM1)<us);
	TIM_Cmd(TIM1,DISABLE);
}