/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   *SPI3
			*SPI1_SCK: PC10
			*SPI1_MOSI: PC12
			*SPI1_MISO: PC11
			*CS: PA15
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PS2_H
#define PS2_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
/* Exported types ------------------------------------------------------------*/
typedef enum {PRESSED = 0, RELEASED = !PRESSED} ButtonState;
typedef enum {NOTPASS = 0, PASS = !NOTPASS} TestData;
/* Exported constants --------------------------------------------------------*/
/**
	*@defgroup Digital button 
	*@brief	
	*/
#define PSB_SELECT      ((uint16_t) 0x0001)
#define PSB_L3          ((uint16_t) 0x0002)
#define PSB_R3          ((uint16_t) 0x0004)
#define PSB_START       ((uint16_t) 0x0008)
#define PSB_PAD_UP      ((uint16_t) 0x0010)
#define PSB_PAD_RIGHT   ((uint16_t) 0x0020)
#define PSB_PAD_DOWN    ((uint16_t) 0x0040)
#define PSB_PAD_LEFT    ((uint16_t) 0x0080)
#define PSB_L2          ((uint16_t) 0x0100)
#define PSB_R2          ((uint16_t) 0x0200)
#define PSB_L1          ((uint16_t) 0x0400)
#define PSB_R1          ((uint16_t) 0x0800)
#define PSB_GREEN       ((uint16_t) 0x1000)
#define PSB_RED         ((uint16_t) 0x2000)
#define PSB_BLUE        ((uint16_t) 0x4000)
#define PSB_PINK        ((uint16_t) 0x8000)
#define PSB_TRIANGLE    ((uint16_t) 0x1000)
#define PSB_CIRCLE      ((uint16_t) 0x2000)
#define PSB_CROSS       ((uint16_t) 0x4000)
#define PSB_SQUARE      ((uint16_t) 0x8000)
/**
	*@defgroup Analog joystick
	*@brief	
	*/
#define PSS_RX 5
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8
/**
	*@defgroup Analog button
	*@brief	
	*/

//These are analog buttons
//#define PSAB_PAD_RIGHT    9
//#define PSAB_PAD_UP      11
//#define PSAB_PAD_DOWN    12
//#define PSAB_PAD_LEFT    10
//#define PSAB_L2          19
//#define PSAB_R2          20
//#define PSAB_L1          17
//#define PSAB_R1          18
//#define PSAB_GREEN       13
//#define PSAB_RED         14
//#define PSAB_BLUE        15
//#define PSAB_PINK        16
//#define PSAB_TRIANGLE    13
//#define PSAB_CIRCLE      14
//#define PSAB_CROSS       15
//#define PSAB_SQUARE      16
/**
	*@defgroup PS2 modes
	*@brief	
	*/

#define MODE_DIGITAL  ((uint8_t) 0x41)
#define MODE_ANALOG   ((uint8_t) 7<<4)
#define MODE_CONFIG   ((uint8_t) (0xF<<4))

#define POLLING 			((uint8_t) 0x42)
#define CONFIG_MODE 	((uint8_t) 0x43)
#define DIG_ANL 			((uint8_t) 0x44)

#define SPI_BUFFERSIZE 5
/* Exported macro ------------------------------------------------------------*/
/**
	*@defgroup Chip select macros
	*@brief	
	*/
#define DESELECT() (GPIO_SetBits(GPIOA, GPIO_Pin_15))
#define SELECT() (GPIO_ResetBits(GPIOA, GPIO_Pin_15))
/* Exported functions ------------------------------------------------------- */

void PS2_Init(void);
void DMAConfig(void);
void FirstHandshake(void);
void WaitForTransmission(void);
void RequestData(void);
ButtonState Is_Pressed(uint16_t BUTTON_NAME);
TestData CheckDataReceived(void);
void SPI3_IRQHandler(void);

#endif /* PS2_H */

/************************ (C) COPYRIGHT VIAMLAB *****END OF FILE****/
