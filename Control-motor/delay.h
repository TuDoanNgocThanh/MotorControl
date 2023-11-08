
/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USE_DELAY_H
#define __USE_DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void Config(void) __attribute__ ((constructor));
void delay_ms (uint32_t DelayTime);
void delay_us (uint32_t DelayTime);


#endif /* __USE_DELAY_H */

/************************ (C) COPYRIGHT VIAMLAB *****END OF FILE****/
