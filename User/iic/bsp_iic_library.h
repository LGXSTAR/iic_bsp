#ifndef _BSP_IIC_LIBRARY_H
#define _BSP_IIC_LIBRARY_H

#include "stm32f10x.h"
#include "bsp_dma_mtp.h"

/*
***IIC EEPROM���Ŷ���***

*/

#define  I2C_FLAG_LONGTime             (uint32_t)0x2710

/*
Ϊ��4������ʾ������ʼ�ź�λ��
Ϊ ��5������ʾ���ڷ���Ӳ����ַλ��
Ϊ ��6������ʾ���ڷ���ROM�洢��ַλ��
Ϊ ��7������ʾ���ڷ�����Ч����λ��
Ϊ ��8������ʾ���ڽ�������λ��
*/
#define  EEPROM_ERROR_FEEDBACK(n)      printf("\r\n��ʱ�����������Ϊ��%d\r\n",n)   

#define  EEPROM_ADDR                    0xA0      //EEPROMӲ����ַ
#define  STM32_I2C_OWN_ADDR             0x6A

#define  EEPROM_I2C                     I2C1
#define  EEPROM_I2C_CLK                 RCC_APB1Periph_I2C1
#define  EEPROM_I2C_APBxClkCmd          RCC_APB1PeriphClockCmd
#define  EEPROM_I2C_BAUDRATE            400000

// IIC GPIO ���ź궨��
#define  EEPROM_I2C_SCL_GPIO_CLK       (RCC_APB2Periph_GPIOB)
#define  EEPROM_I2C_SDA_GPIO_CLK       (RCC_APB2Periph_GPIOB)
#define  EEPROM_I2C_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd
    
#define  EEPROM_I2C_SCL_GPIO_PORT       GPIOB   
#define  EEPROM_I2C_SCL_GPIO_PIN        GPIO_Pin_6

#define  EEPROM_I2C_SDA_GPIO_PORT       GPIOB
#define  EEPROM_I2C_SDA_GPIO_PIN        GPIO_Pin_7



void I2C_EE_Config(void);
uint32_t EEPROM_Byte_Write(uint8_t addr,uint8_t data);
uint32_t EEPROM_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead);
uint32_t EEPROM_WaitForWriteEnd(void);
uint32_t EEPROM_Page_Write(uint8_t addr,uint8_t *data,uint8_t numByteToWrite);

#endif /*_BSP_IIC_LIBRARY_H*/

