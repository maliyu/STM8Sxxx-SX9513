/**
  ******************************************************************************
  * @file main.c
  * @brief This file contains the main function for this template.
  * @author STMicroelectronics - MCD Application Team
  * @version V2.0.0
  * @date 15-March-2011
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "sx9513.h"
//#include "myI2C.h"
#include "soft_i2c.h"
#include "misc.h"

/* Private defines -----------------------------------------------------------*/
#define FIRMWARE_VERSION        "2.1.0"
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static uint32_t cnt = 0;

void main(void)
{   
  /* disable interrupts */
  disableInterrupts();
    
  /* Set clock as internal 16Mhz clock */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  // LD1
  GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
  
  /* PD7 as NRIQ of SX9513 */
  GPIO_Init(PORT_NIRQ, PIN_NIRQ, GPIO_MODE_IN_PU_IT);
  /* wait until NIRQ pin become high */
  //while(GPIO_ReadInputPin(PORT_NIRQ, PIN_NIRQ) == RESET);
  //EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
  EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
  
  /* I2C init */
#ifdef SOFT_I2C
  Soft_I2C_Init();
#else
  I2C_Cmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
  //GPIO_Init(PORT_SCL, PIN_SCL, GPIO_MODE_IN_FL_NO_IT);
  //GPIO_Init(PORT_SDA, PIN_SDA, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(PORT_SCL, PIN_SCL, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(PORT_SDA, PIN_SDA, GPIO_MODE_OUT_PP_HIGH_FAST);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
  GPIO_ExternalPullUpConfig(PORT_SCL, PIN_SCL, ENABLE);
  GPIO_ExternalPullUpConfig(PORT_SDA, PIN_SDA, ENABLE);
  I2C_Init(STANDARDSPEED, 0xA0, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, CLK_GetClockFreq()/1000000);
  //I2C_ITConfig(I2C_IT_ERR, ENABLE);
  //I2C_ITConfig(I2C_IT_EVT, ENABLE);
  //I2C_ITConfig(I2C_IT_BUF, ENABLE);
  I2C_AcknowledgeConfig(I2C_ACK_CURR);
  I2C_Cmd(ENABLE);
#endif
  
  SX9513_Init();
  
  /* enable interrupts */
  enableInterrupts();
  
  /* Infinite loop */
  while (1)
  {
    if(cnt++>500000)
    {
      SX9513_Tuner();
      
      GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
      
      cnt = 0;
    }
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
