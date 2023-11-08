/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "delay.h"
//#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  This function configures timer.
  * @param  None
  * @retval None
  */

void Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision 			= TIM_CKD_DIV4;
	TIM_TimeBaseInitStruct.TIM_CounterMode 				= TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler 					= 0;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter 	= 0;
	TIM_TimeBaseInitStruct.TIM_Period 						= 41;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM6, ENABLE);
	
	
}
/**
  * @brief  This function creates delay in microsecond.
  * @param  Delay time in microsecond
  * @retval None
  */

void delay_us(uint32_t DelayTime){
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	while (DelayTime){
		while (TIM_GetITStatus(TIM6, TIM_IT_Update) == RESET);
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
			DelayTime--;	
	}
}

/**
  * @brief  This function creates delay in milisecond.
  * @param  Delay time in milisecond
  * @retval None
  */
void delay_ms(uint32_t DelayTime){
	delay_us(DelayTime*1000);
}


/************************ (C) COPYRIGHT Doan Ngoc Thanh Tu *****END OF FILE****/
