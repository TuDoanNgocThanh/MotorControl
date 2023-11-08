/**
  ******************************************************************************
  * @file    pwm.h
  * @author  Nguyen Huynh Nguyen Loc
  * @version V0.0.1
  * @date    31-08-2020
  * @brief   This file contains the headers of the pwm control.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PWM_H
#define PWM_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void PWMInit(void);
void setSpeed1(uint16_t dutycycle);
void setSpeed2(uint16_t dutycycle);
void setSpeed3(uint16_t dutycycle);
void setSpeed4(uint16_t dutycycle);

#endif /* PWM_H */
/************************ (C) COPYRIGHT VIAMLAB *****END OF FILE****/
