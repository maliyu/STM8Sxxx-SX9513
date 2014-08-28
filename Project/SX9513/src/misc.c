/******************************************************************************
 * Project        : STM8Sxxx-SX9513
 * File           : misc.c
 * Copyright      : 2014 Yosun Singapore Pte Ltd
 ******************************************************************************
  miscellaneous function implementation

  Change History:

    Version 1.0.0 - Aug 2014
    > Initial revision

******************************************************************************/
#include "stm8s.h"
#include "sx9513.h"

static uint8_t IrqSrc;
static uint8_t PreTouchStatus;
static uint8_t TouchStatus;
#ifdef SX9513_TUNE
static uint8_t CapSenseUsefulDataMsb;
static uint8_t CapSenseUsefulDataLsb;
static uint8_t CapSenseAverageDataMsb;
static uint8_t CapSenseAverageDataLsb;
static uint8_t CapSenseDiffDataMsb;
static uint8_t CapSenseDiffDataLsb;
static uint8_t CapSenseCompMsb;
static uint8_t CapSenseCompLsb;
#endif
static uint8_t PowerFunc = 0;
static uint8_t SpeedFunc = 0;
static uint8_t TimerFunc = 0;

void delay_us(u8 time)
{
  u8 i=0;
  do
  {
    for(i=0;i<9;i++) /*2???©P´Á*/
    nop();           /*1???©P´Á*/
  }while(time--);
}

/* BL0 - power button with LED0 */
/* BL1 - speed+ button with LED1, LED2, LED3 */
/* BL2 - speed- button with LED1, LED2, LED3 */
/* BL3 - timer button with LED4, LED5, LED6, LED7 */
void SX9513_Handler(void)
{
	uint8_t tmp;
	
	IrqSrc = SX9513_Read(0x00);
	TouchStatus = SX9513_Read(0x01);
	
	if((IrqSrc & 0x40) == 0x40)
	{
		// detect touch event
		if((TouchStatus & 0x01) == 0x01)
		{
			// BL0
			// LED0 on, LED1 on, other LED off
			//STM_EVAL_LEDToggle(LED3);
			if(PowerFunc == 0)
			{
				SX9513_Write(0x0C, 0x03);
				SX9513_Write(0x10, 0xFF);
				SX9513_Write(0x1E, 0x0F);
				PowerFunc = 1;
				SpeedFunc = 1;
				TimerFunc = 0;
			}
			else
			{
				SX9513_Write(0x0C, 0x00);
				SX9513_Write(0x10, 0x00);
				SX9513_Write(0x1E, 0x01);
				PowerFunc = 0;
			}
		}
		else if((TouchStatus & 0x02) == 0x02)
		{
			// BL1
			if(SpeedFunc == 1)
			{
				// LED1 off, LED2 on, LED3 off
				tmp = SX9513_Read(0x0C);
				tmp &= 0xF1;
				tmp |= 0x04;
				SX9513_Write(0x0C, tmp);
				SpeedFunc = 2;
			}
			else if(SpeedFunc == 2)
			{
				// LED1 off, LED2 off, LED3 on
				tmp = SX9513_Read(0x0C);
				tmp &= 0xF1;
				tmp |= 0x08;
				SX9513_Write(0x0C, tmp);
				SpeedFunc = 3;
			}
		}
		else if((TouchStatus & 0x04) == 0x04)
		{
			// BL2
			if(SpeedFunc == 3)
			{
				// LED1 off, LED2 on, LED3 off
				tmp = SX9513_Read(0x0C);
				tmp &= 0xF1;
				tmp |= 0x04;
				SX9513_Write(0x0C, tmp);
				SpeedFunc = 2;
			}
			else if(SpeedFunc == 2)
			{
				// LED1 on, LED2 off, LED3 off
				tmp = SX9513_Read(0x0C);
				tmp &= 0xF1;
				tmp |= 0x02;
				SX9513_Write(0x0C, tmp);
				SpeedFunc = 1;
			}
		}
		else if((TouchStatus & 0x08) == 0x08)
		{
			// BL3
			if(TimerFunc == 0)
			{
				// LED4 on, LED5 off, LED6 off, LED7 off
				tmp = SX9513_Read(0x0C);
				tmp &= 0x0F;
				tmp |= 0x10;
				SX9513_Write(0x0C, tmp);
				TimerFunc = 1;
			}
			else if(TimerFunc == 1)
			{
				// LED4 off, LED5 on, LED6 off, LED7 off
				tmp = SX9513_Read(0x0C);
				tmp &= 0x0F;
				tmp |= 0x20;
				SX9513_Write(0x0C, tmp);
				TimerFunc = 2;
			}
			else if(TimerFunc == 2)
			{
				// LED4 off, LED5 off, LED6 on, LED7 off
				tmp = SX9513_Read(0x0C);
				tmp &= 0x0F;
				tmp |= 0x40;
				SX9513_Write(0x0C, tmp);
				TimerFunc = 3;
			}
			else if(TimerFunc == 3)
			{
				// LED4 off, LED5 off, LED6 off, LED7 on
				tmp = SX9513_Read(0x0C);
				tmp &= 0x0F;
				tmp |= 0x80;
				SX9513_Write(0x0C, tmp);
				TimerFunc = 0;
			}
		}
	}
	else if((IrqSrc & 0x20) == 0x20)
	{
		// detect release event
		if(((TouchStatus & 0x01) == 0x00) && ((PreTouchStatus & 0x01) == 0x01) )
		{
			// BL0
			//STM_EVAL_LEDToggle(LED3);
			
		}
	}
	
	PreTouchStatus = TouchStatus;
}
