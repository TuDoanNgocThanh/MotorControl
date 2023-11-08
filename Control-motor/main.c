#include "stm32f4xx.h"            
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_SIZE_T 6
#define DATA_SIZE_R 18
char dataR[DATA_SIZE_R];
char dataT[DATA_SIZE_T];
char opening,ending;



int8_t  str[8];
uint8_t i=0,a;
int32_t Counter1,Counter2,check=0;
int dem=0,cnt=0;
int z=-1,mode,Clockwise_old=1,Clockwise_new;
double	speed_old=0,speed_new,out;
//SKpPKiIKdDSetE
double Duty_old=0,Duty_new,Ts=0.0003,t;
 //float Kp=0.1,Ki=0.3/5,Kd=0.0003,T=0.02;
float Kp=0.7,Ki=0.7,Kd=0.0003,T=0.025,speed_nhan,checksum;
float duty,duty_old=0,duty_set,speed_set,pre_output=0,Output,speed_send;
float Error_Current=0,Speed_Current,Error_Past=0,set_tmp=0,pre_set_speed;
float Error_Derivative=0,Error_Sum=0,duty_current=10,U,Out;
int m;
int chuoi;

union float32bit
{		float send;
    char sendbyte[4];  // flaot 32 bit to 4 char 8 bit
}gui, nhan;

GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure1;
USART_InitTypeDef USART_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

void	SETPWM(double duty);
void USART_Config(int BaudRate);
void USART_Send_String(volatile char *String);
void USART2_IRQHandler(void);
void delay_us(uint32_t timedelay);
void Config(void);
volatile int32_t debug; 
void GPIO_Configuration(void); 
void TIM4_PWM_Configuration(void); 
void TIM_ENCODER_Config(void);
void TIM3_IRQHandler(void);
void TIM_IC_Config(void);
double PID(void);

											/*
												PC6, PC7 ENCODER
												PB6, PB7 DMA
												PD12, PD13 PWM
											*/
int main()
	{ 

	GPIO_Configuration(); 
	USART_Config(115200);	
	TIM4_PWM_Configuration();
	TIM_IC_Config();	
	TIM_ENCODER_Config();
	
		while(1){		
						}
}	

void TIM_IC_Config(void)	//read encoder PC6/PC7
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);// PC6
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);// PC7
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_Period =0xFFFF ;  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter=15;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
//	TIM_ICInitStructure.TIM_ICFilter0;
	TIM_ICInitStructure.TIM_ICFilter=15;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;

	
	TIM_EncoderInterfaceConfig(TIM8,TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);  // use both edge of 2 chanel
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	
	TIM_SetCounter(TIM8, 20000);
	TIM_Cmd(TIM8, ENABLE);
}


void USART_Config(int BaudRate)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	

	// Initialize pins as alternating function
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;  //TXD is PA3 RXD is PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	USART_InitStructure.USART_BaudRate=BaudRate;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	
	//cho phep su dung ngat receive
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 
  NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(USART2,ENABLE);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	DMA_InitTypeDef DMA_InitStructure;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //khung 8 bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1 stop bit
	USART_InitStructure.USART_Parity = USART_Parity_No; //no parity
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //vo hieu hoa dong dk phan cung
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); //cho phép uart

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);
	USART_Cmd(USART1, ENABLE);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); 

/* DMA2 Stream7 Channel4 for UART1 Tx configuration */			
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)dataT;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = DATA_SIZE_T;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// don't increase peripheral address 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // tang dia chi bo nho de gui uart
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //send mode Normal, recevice mode Circular
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream7, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream7, ENABLE);

/* DMA2 Stream5 Channel4 for UART1 Rx configuration */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)dataR; 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = DATA_SIZE_R;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//  don't increase peripheral address 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //increase momery address
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream5, &DMA_InitStructure);

  DMA_Cmd(DMA2_Stream5, ENABLE);
  USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);

	  /* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

//  /* Enable the UART1 TX DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
	
//	  /* Enable the UART1 RX DMA Interrupt */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
		
}

void DMA2_Stream5_IRQHandler(void)
	{
		if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5) != RESET)
		{
			//Frame recieve "A+mode+Kp+Ki+Kd+SetPoint+duty+B"
			DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
			memcpy(&opening, dataR, 1);
			memcpy(&ending, dataR + 17, 1);
			if((opening == 'A') && (ending == 'B'))
			{
				memcpy(&Kp, dataR + 1, 4);
				memcpy(&Ki, dataR + 5, 4);
				memcpy(&Kd, dataR + 9, 4);
				memcpy(&speed_nhan, dataR + 13, 4);		
			}
			DMA_SetCurrDataCounter(DMA2_Stream5, DATA_SIZE_R);
			DMA_Cmd(DMA2_Stream5, ENABLE);
		}
	}

	
	void TIM3_IRQHandler(void) //interrup timer3 20MS: read encoder, calculate PID -> set PWM 
{	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		speed_set=speed_nhan;
		
				TIM_Cmd(TIM8, DISABLE);
				Counter1=TIM_GetCounter(TIM8);
				Speed_Current=((double)(Counter1-20000)/(374.0*0.02*2))*60;		
		U=PID();
		SETPWM(U);
		dem++;
		if(dem==1){
			dem=0;
						DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
						DMA_Cmd(DMA2_Stream7, DISABLE);
						//send Frame : [Y][speed][E]
						dataT[0] = 'Y';
						memcpy(dataT + 1, &Speed_Current, 4);
						dataT[5] = '\n';
						DMA2_Stream7->NDTR = DATA_SIZE_T;	// number byte send 
						DMA_Cmd(DMA2_Stream7, ENABLE);		// enable DMA
						TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		}
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_SetCounter(TIM8, 20000);
	/*Enable 1nd Timer */
	TIM_Cmd(TIM8,ENABLE);
}


double PID(void)
{	
pre_output=Output;
	Error_Current = speed_set-Speed_Current;

	Error_Sum = Error_Sum + Error_Current;
	Error_Derivative = Error_Current-Error_Past;

	Output = Kp*Error_Current+Ki*Error_Sum*T+Kd*Error_Derivative/T;
	Error_Past = Error_Current;
	if(Output >99)
		Output=99;
	if(Output<-99)
		Output=-99;
	if(abs(Output) <10)
		Output=0;
	if(abs(Error_Current)<8)
		Output=pre_output;
		return Output; //duty
}

void TIM_ENCODER_Config(void) //TIM3, 20MS
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler=83; // 1Mhz
	TIM_TimeBaseStructure.TIM_Period=20000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision=0;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE); 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void	SETPWM(double duty)
{
if(pre_output>0 && Output<0 || pre_output<0 && Output>0)
					{	TIM4->CCR2 = 0;
						TIM4->CCR1 = 0;
						delay_ms(10);}
				if(duty >0)
					{
						TIM4->CCR2 = duty*65535.0/100.0;
						TIM4->CCR1 = 0;
					}
					else
					{
						TIM4->CCR1 = -duty*65535.0/100.0;
						TIM4->CCR2= 0;
					}	
	
}

void TIM4_PWM_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler=0; //+1
	TIM_TimeBaseStructure.TIM_ClockDivision=0;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=65536-1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse=0;



	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}

void GPIO_Configuration(void) 
	{ 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); /* Configure PB0 PB1 in output pushpull mode */ 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
		GPIO_Init(GPIOD, &GPIO_InitStructure); 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
		GPIO_Init(GPIOA, &GPIO_InitStructure); 
	} 
	/************************ (C) COPYRIGHT Doan Ngoc Thanh Tu *****END OF FILE****/
