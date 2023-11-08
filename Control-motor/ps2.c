/**
  ******************************************************************************
  * @file    ps2.c 
  * @author  Nguyen Huynh Nguyen Loc
  * @version V0.0.2
  * @date    06-09-2020
  * @brief   PS2 SPI Interface
  ******************************************************************************
  * @attention
  *
  */
/* Includes ------------------------------------------------------------------*/
#include "ps2.h"
#include "uart_termite.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SS_DIS GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define SS_EN GPIO_ResetBits(GPIOA, GPIO_Pin_15)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern __IO uint8_t aRxBuffer[SPI_BUFFERSIZE];
extern __IO uint8_t ubRxIndex;
extern __IO uint8_t ubTxIndex;
extern __IO uint8_t aTxBuffer[SPI_BUFFERSIZE];
extern __IO uint32_t TimeOut;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  Khoi tao SPI, GPIO, Interrupt cau hinh cho PS2. Chay ham nay truoc main.
  * @param  None
  * @retval None
  */
	
void PS2_Init(void){
	//Buffer counter initial
	ubTxIndex = 0;
	ubRxIndex = 0;
	// SPI3 GPIO Pin init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef  	SPI3_GPIOInitStruct;
	SPI3_GPIOInitStruct.GPIO_Mode 	= GPIO_Mode_AF;
	SPI3_GPIOInitStruct.GPIO_OType 	= GPIO_OType_PP;
	
	SPI3_GPIOInitStruct.GPIO_Speed 	= GPIO_Speed_100MHz;
	//SCK, MISO, MOSI
	SPI3_GPIOInitStruct.GPIO_PuPd		= GPIO_PuPd_UP;
	SPI3_GPIOInitStruct.GPIO_Pin 		=  GPIO_Pin_11 |  GPIO_Pin_12;
	GPIO_Init(GPIOC, &SPI3_GPIOInitStruct);
	
	//SPI3_GPIOInitStruct.GPIO_OType	= ;
	SPI3_GPIOInitStruct.GPIO_PuPd		= GPIO_PuPd_DOWN;
	SPI3_GPIOInitStruct.GPIO_Pin 		=  GPIO_Pin_10;
	GPIO_Init(GPIOC, &SPI3_GPIOInitStruct);
	//NCS
	SPI3_GPIOInitStruct.GPIO_Mode		= GPIO_Mode_OUT;
	SPI3_GPIOInitStruct.GPIO_Pin 		=	GPIO_Pin_15;
	GPIO_Init(GPIOA, &SPI3_GPIOInitStruct);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
	//SPI Config
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	SPI_I2S_DeInit(SPI3);
	SPI_InitTypeDef 		SPI3_InitStruct;
	SPI3_InitStruct.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;
	SPI3_InitStruct.SPI_Mode 							= SPI_Mode_Master;
	SPI3_InitStruct.SPI_DataSize 					= SPI_DataSize_8b;
	SPI3_InitStruct.SPI_FirstBit 					= SPI_FirstBit_LSB;
	SPI3_InitStruct.SPI_CPOL							= SPI_CPOL_High;
	SPI3_InitStruct.SPI_CPHA							= SPI_CPHA_1Edge;
	SPI3_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI3_InitStruct.SPI_NSS								= SPI_NSS_Soft;
	SPI_Init(SPI3, &SPI3_InitStruct);
	// Interrupt priority config
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef 			SPI_NVIC_InitStruct;
	SPI_NVIC_InitStruct.NVIC_IRQChannel = SPI3_IRQn;
	SPI_NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	SPI_NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	SPI_NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&SPI_NVIC_InitStruct);
}



/**
  * @brief  Don't care about this function. It will be make used of later.
  * @param  None
  * @retval None
  */

void FirstHandshake(void){
	SPI_Cmd(SPI3, DISABLE);
	aTxBuffer[0] = 0x01;
	aTxBuffer[1] = 0x43;
	aTxBuffer[2] = 0x00;
	aTxBuffer[3] = 0x00;
	aTxBuffer[4] = 0x00;
	SPI_Cmd(SPI3, ENABLE);
}

/**
  * @brief  Gui command toi PS2 va nhan ve du lieu.
  * @param  None
  * @retval None
  */
void RequestData(void){
	aTxBuffer[0] = 0x01;
	aTxBuffer[1] = 0x42;
	aTxBuffer[2] = 0x00;
	aTxBuffer[3] = 0x00;
	aTxBuffer[4] = 0x00;
	ubTxIndex = 0;
	ubRxIndex = 0;
	SS_EN;
	SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, ENABLE);
	SPI_Cmd(SPI3, ENABLE);
	for (ubTxIndex = 0; ubTxIndex < SPI_BUFFERSIZE; ubTxIndex++){
		SPI_SendData(SPI3, aTxBuffer[ubTxIndex]);
		while (SPI_GetFlagStatus(SPI3, SPI_FLAG_TXE)!=SET); 
	}
	while (SPI_GetFlagStatus(SPI3, SPI_FLAG_TXE)!=SET); 
	while (SPI_GetFlagStatus(SPI3, SPI_FLAG_BSY)!=RESET);
	SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, DISABLE);
	SPI_Cmd(SPI3, DISABLE);
	SS_DIS;
}


/**
  * @brief  Kiem tra nut bam dang nhan.
  * @param  Ten nut nhan. (tham khao muc Exported Constant trong file ps2.h
  * @retval Trang thai nut nhan (PRESSED or RELEASED)
  */
ButtonState Is_Pressed(uint16_t BUTTON_NAME){
		ButtonState returnValue = RELEASED;
		uint16_t BUTTONDATA = (aRxBuffer[4] << 8) |  (aRxBuffer[3]);
		uint16_t buttonState = BUTTONDATA & BUTTON_NAME;
		if (buttonState)
				returnValue = RELEASED;
		else 
				returnValue = PRESSED;
		return returnValue;
}

TestData CheckDataReceived(void){
	if (aRxBuffer[0] == 0xFF && aRxBuffer[1] == 0x41 && aRxBuffer[2] == 0x5A)
		return PASS;
	else return NOTPASS;
		
}

/**
  * @brief  SPI Interrupt handler. If done receiving, an interrupt event occurs and data received will be stored in ubTxBuffer
  * @param  None
  * @retval None
  */


void SPI3_IRQHandler(void)
{
	/* SPI in Transmitter mode */
	//while (SPI_GetFlagStatus(SPI3, SPI_FLAG_RXNE)!=SET);
  if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET)
  {
    if (ubRxIndex < SPI_BUFFERSIZE)
    {
      /* Receive Transaction data */
      aRxBuffer[ubRxIndex++] = SPI_ReceiveData(SPI3);
			//SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, DISABLE);
    }
    else
    {
      /* Disable the Rx buffer not empty interrupt */
      SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, DISABLE);
    }
		//SendUSART("RX");
  }
}
/************************ (C) COPYRIGHT VIAMLAB *****END OF FILE****/
