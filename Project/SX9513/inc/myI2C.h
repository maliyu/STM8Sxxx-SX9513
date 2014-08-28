/******************************************************************************
 * Project        : STM8Sxxx-SX9513
 * File           : myI2C.h
 * Copyright      : 2014 Yosun Singapore Pte Ltd
 ******************************************************************************
  Soft I2C implementation

  Change History:

    Version 1.0.0 - Aug 2014
    > Initial revision

******************************************************************************/
#ifndef _MYI2C_H_
#define _MYI2C_H_

void myI2C_Init(void);
void SX9513_Write(unsigned char regadd,unsigned char wdata);
unsigned char SX9513_Read(unsigned char regadd);

#endif
