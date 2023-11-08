#include "variable.h"

 uint8_t USART2_Tx_Data[17];
 uint8_t USART2_Rx_Data[17];
 
 // delay
 uint16_t ms;
 uint16_t us;
 
 // PID
 float Kp,Ki,Kd,Current_Speed,Set_Speed;
int	A_Send = 0;
float Output,Pre_Output,T=0.01;
float	Error_Derivative,Error_Sum,Error_Current,Error_Past;