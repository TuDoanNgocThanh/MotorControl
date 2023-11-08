/**
  ******************************************************************************
  * @file    USART/USART_TwoBoards/USART_DataExchangeInterrupt/main.h
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_TERMITE_H
#define __UART_TERMITE_H



/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdio.h"
#include "delay.h"
#include "string.h"

/* Exported typedef -----------------------------------------------------------*/
/* Misc definition ************************************************************/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define BUFFERSIZE                     255
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USARTConfig(uint16_t);
void SendUSART(char* data);
char* GetUSART(void);
int GetInt(void);

#endif /* __UART_TERMITE_H */

/************************ (C) COPYRIGHT VIAMLAB *****END OF FILE****/
