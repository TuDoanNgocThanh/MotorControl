#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"

/*Send data*/

extern uint8_t USART2_Tx_Data[17];
extern uint8_t USART2_Rx_Data[17];

//	delay
extern uint16_t ms;
extern uint16_t us;
// PID
extern float Kp,Ki,Kd,Current_Speed,Set_Speed;
extern float Output,Pre_Output,T;
	
extern float	Error_Derivative,Error_Sum,Error_Current,Error_Past ;

extern int A_Send;