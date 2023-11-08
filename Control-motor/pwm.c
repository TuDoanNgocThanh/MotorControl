/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   
							*TIM3
							*CH1 PC6
							*CH2 PC7
							*CH3 PC8
							*CH4 PC9
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pwm.h"

/** @addtogroup PWM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static int period;
/* Private function prototypes -----------------------------------------------*/
static void GPIOConfig(void);
static void OCConfig(void);
static void TimeBaseConfig(void);
static uint16_t constrain(uint16_t);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Run before while loop to .
  * @param  None
  * @retval None
  */

void PWMInit(void) {
	GPIOConfig();
	TimeBaseConfig();
	OCConfig();
	
}

/**
  * @brief  This function configures GPIO. In here we use GPIOC and pin pack 1.
  * @param  None
  * @retval None
  */

static void GPIOConfig(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef 		PWM_GPIOInitStruct;
	PWM_GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF;
	PWM_GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
	PWM_GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	PWM_GPIOInitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	PWM_GPIOInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC, &PWM_GPIOInitStruct);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
}

/**
  * @brief  This function configures timer.
  * @param  None
  * @retval None
  */

static void TimeBaseConfig(void){
	period = 20000-1;
	TIM_TimeBaseInitTypeDef PWM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	PWM_TimeBaseInitStruct.TIM_ClockDivision 			= TIM_CKD_DIV1;
	PWM_TimeBaseInitStruct.TIM_Prescaler     			= 83;
	PWM_TimeBaseInitStruct.TIM_CounterMode 	 			= TIM_CounterMode_Up;
	PWM_TimeBaseInitStruct.TIM_RepetitionCounter 	= 0;
	PWM_TimeBaseInitStruct.TIM_Period 						= period; 
	TIM_TimeBaseInit(TIM3, &PWM_TimeBaseInitStruct);
}
/**
  * @brief  This function configures output compare.
  * @param  None
  * @retval None
  */


static void OCConfig(void){
	TIM_OCInitTypeDef PWM_TIMOCInitStruct;
	PWM_TIMOCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	PWM_TIMOCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	PWM_TIMOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	PWM_TIMOCInitStruct.TIM_Pulse  = 1900;
	TIM_OC1Init(TIM3, &PWM_TIMOCInitStruct);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM3, &PWM_TIMOCInitStruct);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC3Init(TIM3, &PWM_TIMOCInitStruct);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM3, &PWM_TIMOCInitStruct);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}


/**
  * @brief  Set speed for motor1.
  * @param  Speed percentage
  * @retval None
  */

void setSpeed1(uint16_t pulseValue){
	//uint16_t pulseValue =  (period+1)* constrain(percentage)/100;
	pulseValue = constrain(pulseValue);
	TIM_SetCompare1(TIM3, pulseValue);
}


/**
  * @brief  Set speed for motor2.
  * @param  Speed percentage
  * @retval None
  */


void setSpeed2(uint16_t pulseValue){
	//uint16_t pulseValue = (period+1) * constrain(percentage)/100;
	pulseValue = constrain(pulseValue);
	TIM_SetCompare2(TIM3, pulseValue);
}

/**
  * @brief  Set speed for motor3.
  * @param  Speed percentage
  * @retval None
  */

void setSpeed3(uint16_t pulseValue){
	//uint16_t pulseValue = (period+1) * constrain(percentage)/100;
	pulseValue = constrain(pulseValue);
	TIM_SetCompare3(TIM3, pulseValue);
}


/**
  * @brief  Set speed for motor4
  * @param  Speed percentage
  * @retval None
  */

void setSpeed4(uint16_t pulseValue){
	//uint16_t pulseValue = (period+1) * constrain(percentage)/100;
	pulseValue = constrain(pulseValue);
	TIM_SetCompare4(TIM3, pulseValue);
}

static uint16_t constrain(uint16_t dataIn){
	if (dataIn<999) dataIn = 999;
	if (dataIn>1999) dataIn = 1999;
	return dataIn;
}

/**
  * @brief  Ensure pwm period in range of 5% to 10%.
  * @param  Speed percentage
  * @retval None
  */




