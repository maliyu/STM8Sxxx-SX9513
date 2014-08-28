/******************************************************************************
 * Project        : STM8Sxxx-SX9513
 * File           : myI2C.c
 * Copyright      : 2014 Yosun Singapore Pte Ltd
 ******************************************************************************
  Soft I2C implementation

  Change History:

    Version 1.0.0 - Aug 2014
    > Initial revision

******************************************************************************/
#include <iostm8s105c6.h>
#include "misc.h"

typedef unsigned char uchar;

#define SCL              PA_ODR_ODR3
#define SDA              PA_ODR_ODR4
#define SDAM             PA_IDR_IDR4
#define SET_SCL_OUT()    {PA_CR2_C23 = 0; PA_ODR_ODR3 = 0; PA_DDR_DDR3 = 1; PA_CR1_C13 = 1; PA_CR2_C23 = 0;}
#define SET_SDA_OUT()    {PA_CR2_C24 = 0; PA_ODR_ODR4 = 0; PA_DDR_DDR4 = 1; PA_CR1_C14 = 1; PA_CR2_C24 = 0;}
#define SET_SDA_IN()     {PA_CR2_C24 = 0; PA_DDR_DDR4 = 0; PA_CR1_C14 = 0; PA_CR2_C24 = 0;}
//--------------------------------------------------------------
// Prototype      : void I2C_Start(void)
// Calls          : Delay_5us()
// Description    : Start Singnal
//--------------------------------------------------------------
void myI2C_Init(void)
{
   I2C_CR1 &= 0xFE;      //  PE=0, disable I2C module  
   SET_SCL_OUT();
   SET_SDA_OUT();  
   SCL = 1;
   SDA = 1;
}

//--------------------------------------------------------------
// Prototype      : void Delay_1us(void)
// Description    : ¤j?©µ?5us
//--------------------------------------------------------------
static void Delay_us(unsigned int a)
{
  // 16MHz 1.02us
  // 24MHz 0.68us
  while(a--);
}

//--------------------------------------------------------------
// Prototype      : void Delay_5us(void)
// Description    : ¤j?©µ?5us
//--------------------------------------------------------------
static void Delay_5us(void)
{
   //uchar i;                   
   
   //fcpu 16MHz
   //for (i=100; i>0; i--);
  Delay_us(10);
}

 

//--------------------------------------------------------------
// Prototype      : void I2C_Start(void)
// Calls          : Delay_5us()
// Description    : Start Singnal
//--------------------------------------------------------------
static void I2C_Start(void)
{
    // SDA 1->0 while SCL High
    SDA = 1;  
    Delay_5us(); 
    SCL = 1;                    
    Delay_5us();                
    SDA = 0;                   
    Delay_5us();                
    //SCL = 0;                    
}


//--------------------------------------------------------------
// Prototype      : void I2C_Stop(void)
// Calls          : Delay_5us()
// Description    : Stop Singnal
//-------------------------------------------------------------- 
static void I2C_Stop(void)
{
    // SDA 0->1 while SCL High
    SDA = 0;      
    Delay_5us(); 
    SCL = 1;                   
    Delay_5us();                
    SDA = 1;                   
    //Delay_5us();               
}

//--------------------------------------------------------------
// Prototype      : void I2C_SendACK(uchar ack);
// Calls          : Delay_5us()
// Parameters     : bit ack:1-noack, 0-ack
// Description    : Master device send ACK to slave device.
//--------------------------------------------------------------
static void I2C_SendACK(uchar ack)
{
    if(ack == 0)
    {
      SDA = 0; 
    }  
    else
    {
      SDA = 1; 
    }
    //Delay_5us();   
    SCL = 1;                   
    Delay_5us();  
    
    SCL = 0;                    
    Delay_5us();     
}

//--------------------------------------------------------------
// Prototype      : uchar I2C_SendByte(uchar sendDAT)
// Calls          : Delay_5us()
// Parameters     : uchar sendDAT---data to be send
// Return Value   : CY--slave ack (1---noack¡A0---ack)
// Description    : Send one byte to I2C
//--------------------------------------------------------------
static uchar I2C_SendByte(uchar sendDAT)
{
  uchar i,revack;

  for (i= 0 ; i< 8; i++)         
  {
    SCL = 0;                
    Delay_5us();  

    if (sendDAT & 0x80)             // write data
    {
      SDA = 1;
    }
    else   
    {
      SDA = 0;
    }
    SCL = 1;
    sendDAT <<=  1;
    Delay_5us();
  } 
 
  SCL = 0;
  Delay_5us(); 
  SET_SDA_IN();
  SCL = 1;
  Delay_us(1);
  revack = (uchar)SDAM;  
  Delay_5us();
  SCL = 0;
  SET_SDA_OUT();
  //Delay_5us();

  return revack;
}


//--------------------------------------------------------------
// Prototype      : uchar I2C_RecvByte()
// Calls          : Delay_5us()
// Parameters     : none
// Return Value   : revDAT- received data
// Description    : Receive one byte from I2C
//--------------------------------------------------------------
static uchar I2C_RecvByte(void)
{
    uchar i;
    uchar revDAT = 0;
    
    SDA = 1;     // latch the Data port befor reading
    //SCL = 0;
    SET_SDA_IN();
    
    //Delay_5us();
    
    for (i=0; i<8; i++)         
    { 
       revDAT <<= 1;
       
       SCL = 0; 
       Delay_5us();
       
       SCL = 1;
       if(Get_I2C_SDA())
       {
          revDAT |= 0x01;
       }
       else
       {
          revDAT &= 0xfe;
       }
       //asm("nop");
       //asm("nop");
       //revDAT |= ((uchar)SDAM);
       Delay_5us(); 
       //SCL = 0;                
       //Delay_5us();  
    }
    
    SCL = 0; 
    SET_SDA_OUT();
    //Delay_5us();
   
    return revDAT;
}

//--------------------------------------------------------------
// Prototype      : void Write_PT2314(uchar wdata)
// Calls          : I2C_Start(), I2C_Stop(),I2C_SendByte()
// Parameters     : RegAddr-- target memory address,  
//                  wrdata--- data to be writing
// Description    : Write one byte to target memory
//--------------------------------------------------------------
void SX9513_Write(uchar regadd,uchar wdata)
{
  uchar i2cAck;
  
  I2C_Start();    
  //Delay_5us();
  i2cAck = I2C_SendByte(0x2B<<1);     // Device Addr + Write (operation) 
  /*if(i2cAck)
  {
    return;
  }*/
  i2cAck = I2C_SendByte(regadd);
  /*if(i2cAck)
  {
    return;
  }*/
  i2cAck = I2C_SendByte(wdata);
  /*if(i2cAck)
  {
    return;
  }*/
  I2C_Stop();             
}

uchar SX9513_Read(uchar regadd)
{
    uchar revdata;
    uchar i2cAck;
    
    I2C_Start(); 
    //Delay_5us();
    i2cAck = I2C_SendByte(0x2B<<1);     // Device Addr + Write (operation) 
    /*if(i2cAck)
    {
      return 0xFF;
    }*/
    i2cAck = I2C_SendByte(regadd);
    /*if(i2cAck)
    {
      return 0xFF;
    }*/
    I2C_Start();    
    i2cAck = I2C_SendByte((0x2B<<1)+1);     // Device Addr + Read (operation)   
    /*if(i2cAck)
    {
      return 0xFF;
    }*/
    Delay_5us();
    revdata = I2C_RecvByte();    
    I2C_SendACK(1);
    I2C_Stop();   
    
    return revdata;
}