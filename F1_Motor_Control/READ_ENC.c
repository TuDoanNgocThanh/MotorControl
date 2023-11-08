#include "READ_ENC.h"
void Read_Enc_Config(void)
{
//		/*Enable Clock*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;

//  /* PA8 PA9 */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

//	TIM_TimeBaseStructure.TIM_Prescaler = 0;
//	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
////	TIM_Cmd(TIM1, ENABLE);
//	
//	TIM_ICInitTypeDef TIM_ICInitStructure; 
//	
//  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2;
//  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
//  TIM_ICInitStructure.TIM_ICFilter = 15;
//  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM_ICInit(TIM1, &TIM_ICInitStructure);
//	
//	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);
//	TIM_SetCounter(TIM1,10000);
//	TIM_Cmd(TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	
	GPIO_InitTypeDef GPIO_InitStructure;
  /* PA0 PA1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // = 0
	TIM_TimeBaseStructure.TIM_Period =0xFFFF ;  // su dung toi da thanh ghi ARR
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter=15;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

//	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
//	TIM_ICInitStructure.TIM_ICFilter=15;
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);  // su dung both edge cua 2 kenh

	
	TIM_SetCounter(TIM2, 20000);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_Initstructure;
	NVIC_Initstructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstructure);
}