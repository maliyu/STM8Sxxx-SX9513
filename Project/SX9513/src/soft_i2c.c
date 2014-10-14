/******************** (C) COPYRIGHT  ���iCreateǶ��ʽ���������� **************
 * �ļ���  ��soft_i2c.c
 * ����    ��ģ��I2C������   
 * ʵ��ƽ̨�����STM8������
 * ��汾  ��V2.1.0
 * ����    �����  QQ��779814207
 * ����    ��
 * �Ա�    ��http://shop71177993.taobao.com/
 *�޸�ʱ�� ��2012-12-9

*******************************************************************************/


#include "soft_i2c.h"
#include "misc.h"

#ifdef SOFT_I2C
void Soft_I2C_Init(void)
{
  Set_I2C_SCL_Out;
  Set_I2C_SDA_Out;
  Set_I2C_SCL_HIGHT;
  Set_I2C_SDA_HIGHT;
}
/*********************************************************
 * ����: Soft_I2C_Start
 * ����: ģ��I2C����ʼ����
 * �β�: ��        
 * ����: ��
 * ˵��: ��׼��I2CЭ��I2C��ʼ������SCLΪ��ʱ��SDA�ɸ߱��
               
 *********************************************************/
void Soft_I2C_Start(void)
{
  Set_I2C_SDA_Out;
  Set_I2C_SCL_HIGHT;
  Set_I2C_SDA_HIGHT;
  delay_us(4);
  Set_I2C_SDA_LOW;
  //��׼��I2CЭ��I2C��ʼ������SCLΪ��ʱ��SDA�ɸ߱��
  delay_us(4);
  Set_I2C_SCL_LOW;//ǯסI2C���ߣ�׼�����ͻ�������� 
}
/*********************************************************
 * ����: Soft_I2C_Stop
 * ����: ģ��I2C��ֹͣ����
 * �β�: ��        
 * ����: ��
 * ˵��: ��׼��I2CЭ��I2Cֹͣ������SCLΪ��ʱ��SDA�ɵͱ��
               
 *********************************************************/
void Soft_I2C_Stop(void)
{
  Set_I2C_SDA_Out;
  Set_I2C_SCL_HIGHT;
  Set_I2C_SDA_LOW;
  delay_us(4);
  Set_I2C_SDA_HIGHT;
  //��׼��I2CЭ��I2Cֹͣ������SCLΪ��ʱ��SDA�ɵͱ��
  delay_us(4);

}
/*********************************************************
 * ����: Soft_I2C_Ack
 * ����: ģ��I2C��Ӧ��
 * �β�: ��        
 * ����: ��
 * ˵��: ���ݱ�׼��I2CЭ�飬��I2C��������1��byte��
         Ҳ�����ڵ�9��CLK��ʱ��I2C���豸��SDAҪ���
         ��ʾI2C�������Ѿ�������һ���ֽ�byte��I2C������
         ������������
               
 *********************************************************/
void Soft_I2C_Ack(void)
{
  Set_I2C_SCL_LOW;
  Set_I2C_SDA_Out;
  Set_I2C_SDA_LOW;
  delay_us(2);
  Set_I2C_SCL_HIGHT;
  delay_us(2);
  Set_I2C_SCL_LOW;

}
/*********************************************************
 * ����: Soft_I2C_NAck
 * ����: ģ��I2C�Ĳ�Ӧ��
 * �β�: ��        
 * ����: ��
 * ˵��: 
               
 *********************************************************/
void Soft_I2C_NAck(void)
{
  Set_I2C_SCL_LOW;
  Set_I2C_SDA_Out;
  Set_I2C_SDA_HIGHT;
  delay_us(2);
  Set_I2C_SCL_HIGHT;
  delay_us(2);
  Set_I2C_SCL_LOW;
}
/*********************************************************
 * ����: Soft_I2C_Send_Byte
 * ����: ģ��I2C�����������������һ���ֽ�
 * �β�: Byte        
 * ����: ��
 * ˵��: 
               
 *********************************************************/
void Soft_I2C_Send_Byte(u8 Byte)
{
  u8 bit;
  Set_I2C_SDA_Out;
  Set_I2C_SCL_LOW;
  for(bit=0;bit<8;bit++)
  {
    if((Byte&0x80)>>7)Set_I2C_SDA_HIGHT;
    else Set_I2C_SDA_LOW;
    Byte<<=1;
    delay_us(2);
    Set_I2C_SCL_HIGHT;
    delay_us(2);
    Set_I2C_SCL_LOW;
    delay_us(2);
    
  }
  
}
/************************************************
 * ����: Soft_I2C_Read_Byte
 * ����: ģ��I2C�����������������һ���ֽ�
 * �β�: ��       
 * ����: һ���ֽ�
 * ˵��: �������Ӵ��������һ���ֽ�
               
 **************************************************/
u8 Soft_I2C_Read_Byte(void)
{
  u8 bit,Receive_Byte=0;
  Set_I2C_SDA_In;
  for(bit=0;bit<8;bit++)
  {
    Set_I2C_SCL_LOW;
    delay_us(2);
    Set_I2C_SCL_HIGHT;
    Receive_Byte<<=1;//����һʱ�̵�ֵ����һλ
    if(Get_I2C_SDA)Receive_Byte|=0x01;
    delay_us(1);
  }
  return Receive_Byte;
}
#endif
