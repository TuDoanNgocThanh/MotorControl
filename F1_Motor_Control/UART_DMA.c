#include "UART_DMA.h"
#include "variable.h"

void USART2_Config(uint32_t Baudrate)
{
	// Initialization struct
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// Step 1: Initialize USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStruct.USART_BaudRate = Baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);
	
	/*Enable interrupt DMA Rx */
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
	
	// Step 2: Initialize GPIO for Tx and Rx pin
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Tx pin (PA2) initialization as push-pull alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Rx pin (PA3) initialization as input floating
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
/*Config interrupt DMA Rx USART2*/
void USART2_DMA_Config(uint8_t DMABufferSize) //  PA3 , PA2 
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
		/*Config DMA1 for USART2 Rx*/
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR); // source register
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_Rx_Data;  // destination register
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize =DMABufferSize ;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //8 bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; // 8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);

	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn; //USART 2 Rx - DMA1 Channel 6
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
		
	
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);	
	/* Enable DMA1 Channel 1*/
	DMA_Cmd(DMA1_Channel6, ENABLE);
}