/**
  ******************************************************************************
  * @file    USART and Termite communication
  * @author  Nguyen Huynh Nguyen Loc
  * @version V0.0.1
  * @date    02-09-2020
  * @brief   USART and Termite send/receive.
  *          
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "uart_termite.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

char RXBuffer[BUFFERSIZE];


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  Configure USART1. 
  * @param  Baudrate
  * @retval None
  */

void USARTConfig(uint16_t BaudRate){
	//GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	//TX
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//RX
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//USART
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  Send uart  data to termite.
  * @param  data array
  * @retval None
  */

void SendUSART(char* data){
	for (int i=0; *(data+i)!= NULL; i++){
		USART_SendData(USART1, *(data+i));
		*(data+i) = NULL;
		while (USART_GetFlagStatus(USART1, USART_SR_TXE)==RESET);
	}
}

/**
  * @brief  Get uart data from termite.
  * @param  None
  * @retval None
  */

char* GetUSART(void){
	uint16_t i = 0;
	memset(RXBuffer, NULL, sizeof(RXBuffer));
	for(i=0; i<BUFFERSIZE+1; i++){
		while (USART_GetFlagStatus(USART1, USART_SR_RXNE)==RESET);
		RXBuffer[i] = USART_ReceiveData(USART1);
		if (RXBuffer[i] == 10 || RXBuffer[i]==13) break;
	}
	return RXBuffer;
}

/**
  * @brief  Get integer value received from uart.
  * @param  None
  * @retval None
  */


int GetInt(void){
	char* dataReceive = GetUSART();
	int intData;
	sscanf(dataReceive, "%d", &intData);
	return intData;
}
/************************ (C) COPYRIGHT VIAMLAB *****END OF FILE****/
