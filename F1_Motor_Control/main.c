#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include "variable.h"
#include "PWM.h"
#include "delay.h"
#include "UART_DMA.h"
#include "READ_ENC.h"
/*								TX					RX					TIMER
	INTERUP XU LÍ														TIMER3
	USART2					PA2					PA3
	DMA_USART2									channel6
	PWM							PB8					PB9					TIMER4	//88% 210RPM
	DELAY																		TIMER1
	ENC		TIMER2CH1&CH2		PA0		PA1
	FRAME 	0X31	CMD				OPTION			DATA		SYN		0X30
								MOV				VEL		
								Kp Ki Kd 1 0.7 	0.0003
*/
void send_data(void);
void send_ack(void);
int test=0, test1=0; // for test
int Flag_Speed=0;
//extern union {
//				float a;
//				char b[4];
//							}send_data,get_data;
//void senddata(int lg, char data);
void TIM3_Config(void);
int Counter;
unsigned int cnt_speed = 0;
int	main(void)
{
			TIM3_Config();
			USART2_Config(115200);
			USART2_DMA_Config(17);
			Read_Enc_Config();
			PWM_Config();
			PWM(0,0);
//			delay_ms(500);
//			PWM(0,88);
//			delay_ms(10000);
//			PWM(89,0);
  while (1)
	{	
		send_data();
		send_ack();
//			memcpy(&Kp, USART2_Rx_Data + 11, 4);
//		char *c = "0.5646";
//		double temp = atof(c);
//		test = temp; 	
	}
}
void send_ack(void){
	for(int i =0; i<17;i++){
	USART2_Tx_Data[i] = 0;
	}
				if(A_Send ==1){
			A_Send =0;
					USART2_Tx_Data[0] = '1';
					USART2_Tx_Data[1] = 'S';
					USART2_Tx_Data[2] = 'M';
					USART2_Tx_Data[3] = 'P';
					USART2_Tx_Data[4] = 'I';
					USART2_Tx_Data[5] = 'D';
					USART2_Tx_Data[15] = 'R';
					USART2_Tx_Data[16] = '0';
					for(int cnt_send = 0; cnt_send < 17 ; cnt_send++){
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)!= SET){}
		USART_SendData(USART2,USART2_Tx_Data[cnt_send]);
					}
		}
				else if(A_Send ==2){
				A_Send =0;
					USART2_Tx_Data[0] = '1';
					USART2_Tx_Data[1] = 'S';
					USART2_Tx_Data[2] = 'M';
					USART2_Tx_Data[3] = 'S';
					USART2_Tx_Data[4] = 'P';
					USART2_Tx_Data[5] = 'E';
					USART2_Tx_Data[6] = 'E';
					USART2_Tx_Data[7] = 'D';
					USART2_Tx_Data[15] = 'R';
					USART2_Tx_Data[16] = '0';
					for(int cnt_send = 0; cnt_send < 17 ; cnt_send++){
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)!= SET){}
		USART_SendData(USART2,USART2_Tx_Data[cnt_send]);
				}
			}
}
void send_data(void){

//			cnt_speed++;
////				if(cnt_speed >  100000)
//							if(cnt_speed >  1)
//					{
//						cnt_speed = 0;
			if(Flag_Speed ==1){
				Flag_Speed =0;
					for(int i =0; i<17;i++){
				USART2_Tx_Data[i] = 0;
				}
					USART2_Tx_Data[0] = '1';
					USART2_Tx_Data[1] = 'S';
					USART2_Tx_Data[2] = 'V';
					memcpy(USART2_Tx_Data + 11, &Current_Speed, 4);
					USART2_Tx_Data[15] = 'S';
					USART2_Tx_Data[16] = '0';
					for(int cnt_send = 0; cnt_send < 17 ; cnt_send++){
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)!= SET){}
		USART_SendData(USART2,USART2_Tx_Data[cnt_send]);
				}
				}
					
}
void DMA1_Channel6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC6))
	{
  DMA_ClearITPendingBit(DMA1_IT_TC6);
		if(USART2_Rx_Data[0] == '1' &&  USART2_Rx_Data[16] == '0' && USART2_Rx_Data[1] == 'S' && USART2_Rx_Data[2] == 'M' && USART2_Rx_Data[15] == 'S')
		{

			memcpy(&Kp, USART2_Rx_Data + 3, 4);
			memcpy(&Ki, USART2_Rx_Data + 7, 4);
			memcpy(&Kd, USART2_Rx_Data + 11, 4);
			A_Send = 1;
		}
		if(USART2_Rx_Data[0] == '1' &&  USART2_Rx_Data[16] == '0' && USART2_Rx_Data[1] == 'V' && USART2_Rx_Data[2] == 'M'&& USART2_Rx_Data[15] == 'S')
		{
			A_Send = 2;
			memcpy(&Set_Speed, USART2_Rx_Data + 11, 4);
			
		}
		if(USART2_Rx_Data[0] == '1' &&  USART2_Rx_Data[16] == '0' && USART2_Rx_Data[1] == 'D' && USART2_Rx_Data[2] == 'M'&& USART2_Rx_Data[15] == 'S')
		{
			if(USART2_Rx_Data[3] == 'S' &&  USART2_Rx_Data[4] == 'T' && USART2_Rx_Data[5] == 'O' && USART2_Rx_Data[6] == 'P')
			{Kp=Ki=Kd=Set_Speed=0;}
			if(USART2_Rx_Data[3] == 'R' &&  USART2_Rx_Data[4] == 'E' && USART2_Rx_Data[5] == 'S' && USART2_Rx_Data[6] == 'E' && USART2_Rx_Data[7] == 'T')
			{Kp=Ki=Kd=Set_Speed= Output=Pre_Output=Error_Derivative=Error_Sum=Error_Current=Error_Past=0;}
		}
		
		
		
	DMA_Cmd(DMA1_Channel6, ENABLE);
	}
			USART2_DMA_Config(17);
}

void TIM3_Config(void)	//10ms ngat
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
  TIM_TimeBaseStructure.TIM_Prescaler = 72 -1;// 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 10000-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	/*Enable Interrupt TIM 3 */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	// Interrupt
	NVIC_InitTypeDef NVICStructure;
  NVICStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVICStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVICStructure.NVIC_IRQChannelSubPriority = 1;
  NVICStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICStructure);
}

//void USART3_IRQHandler()
//{
//	if(USART_GetITStatus(USART3,USART_IT_RXNE)!= RESET)
//	{
//		m[cnt]=USART_ReceiveData(USART3);
//		USART_SendData(USART3,m[cnt]);
//		cnt++;
//		if(cnt==10){
//		cnt=0;
//		}
//	}	
//	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//}

/*Interrupt Timer 3 10ms */
void TIM3_IRQHandler()
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{	
//		send_ack();
		TIM_Cmd(TIM2, DISABLE);
		Counter=TIM_GetCounter(TIM2);
		Current_Speed=((float)(Counter-20000)/(374.0*0.01*4))*60;		

		PID();
		if(	(Pre_Output > 0 && Output < 0) || (Pre_Output < 0 && Output>0)	)
					{	PWM(0,0);
						delay_us(20);}
			else if(Output >0)
					{
						PWM(Output,0);
					}
					else
					{
						PWM(0,-Output);
					}	
					cnt_speed++;
					if(cnt_speed == 4){		// 40ms send data 1 lan 
//					send_data();
						Flag_Speed=1;
					cnt_speed =0;}
		TIM_SetCounter(TIM2, 20000);
	/*Enable 1nd Timer */
		TIM_Cmd(TIM2,ENABLE);


					
	}
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
}
