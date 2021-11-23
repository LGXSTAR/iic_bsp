#include "bsp_iic_library.h"

static uint32_t I2C_TIMEOUT_CALL(uint8_t fault);

static void Change_I2C_PIN_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
		//GPIOC_Pin12����Ϊ��©���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_12);

    //GPIOC_Pin11����Ϊ��©���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_11);
}

//void I2C_Pin_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	// ��I2C_GPIO�����ʱ��
//	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_SCL_GPIO_CLK|EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
//	//SCL GPIO����Ϊ��©����ģʽ
//	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

//  //SDA GPIO����Ϊ��©����ģʽ
//	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_GPIO_PIN;
//	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
//}

//void I2C_Config(void)
//{
//	I2C_InitTypeDef  I2C_InitStructure;
//	
//	// ��I2C1��ʱ��
//	EEPROM_I2C_APBxClkCmd(EEPROM_I2C_CLK, ENABLE);

//	// ����IIC�Ĺ�������
//	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;   //ʹ��Ӧ��
//	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //ʹ��7λ��ַģʽ
//	I2C_InitStructure.I2C_ClockSpeed = EEPROM_I2C_BAUDRATE;   //����ʱ��SCLƵ��
//	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  //�ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ�
//	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;  
//	I2C_InitStructure.I2C_OwnAddress1 = STM32_I2C_OWN_ADDR;
//	
//	I2C_Init(EEPROM_I2C,&I2C_InitStructure);   //STM32 IIC�����豸��ַ   ֻҪ�������ϵ�Ψһ��ַ����
//	
//	I2C_Cmd(EEPROM_I2C,ENABLE);
//}


void I2C_EE_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;
	Change_I2C_PIN_Config();
 	
	// ��I2C_GPIO�����ʱ��
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_SCL_GPIO_CLK|EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
	// ��I2C1��ʱ��
	EEPROM_I2C_APBxClkCmd(EEPROM_I2C_CLK, ENABLE);
	
	//SCL GPIO����Ϊ��©����ģʽ
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  //SDA GPIO����Ϊ��©����ģʽ
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_GPIO_PIN;
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);	
	
	// ����IIC�Ĺ�������
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;   //ʹ��Ӧ��
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //ʹ��7λ��ַģʽ
	I2C_InitStructure.I2C_ClockSpeed = EEPROM_I2C_BAUDRATE;   //����ʱ��SCLƵ��
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  //�ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ�
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;  
	I2C_InitStructure.I2C_OwnAddress1 = STM32_I2C_OWN_ADDR;
	
	I2C_Init(EEPROM_I2C,&I2C_InitStructure);   //STM32 IIC�����豸��ַ   ֻҪ�������ϵ�Ψһ��ַ����
	
	I2C_Cmd(EEPROM_I2C,ENABLE);
}

//��EEPROMд��һ���ֽ�

uint32_t EEPROM_Byte_Write(uint8_t addr,uint8_t data)
{
	uint32_t I2C_TimeOut = I2C_FLAG_LONGTime;
	//������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
	}
	
	//EV5�¼�����⵽�������豸��ַ
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	
	I2C_TimeOut = I2C_FLAG_LONGTime;
  while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(5);
	}

	//EV6�¼�����⵽������Ҫ�����Ĵ洢��Ԫ��ַ
	I2C_SendData (EEPROM_I2C,addr);
	
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(6);
	}

  //EV8�¼�����⵽������Ҫ�洢������
	I2C_SendData (EEPROM_I2C,data);
	
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(7);
	}
	
	//���ݴ������
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);	
  return 0;
}



//��EEPROMд�����ֽڣ�ҳд�룩��ÿ��д�벻�ܳ���8���ֽ�

uint32_t EEPROM_Page_Write(uint8_t addr,uint8_t *data,uint8_t numByteToWrite)
{
	uint32_t I2C_TimeOut = I2C_FLAG_LONGTime;
	//������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
	}
	
	//EV5�¼�����⵽�������豸��ַ
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	
	I2C_TimeOut = I2C_FLAG_LONGTime;
  while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(5);
	}
	
	//EV6�¼�����⵽������Ҫ�����Ĵ洢��Ԫ��ַ
	I2C_SendData (EEPROM_I2C,addr);
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(6);
	}

	
	while(numByteToWrite)
	{
		//EV8�¼�����⵽������Ҫ�洢������
		I2C_SendData (EEPROM_I2C,*data);
		
		I2C_TimeOut = I2C_FLAG_LONGTime;
		while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED ) == ERROR)
		{
			I2C_TimeOut--;
		  if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(7);
		}
		
		data++;
		numByteToWrite--;
			
	}
	//���ݴ������
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);	
  return 0;
}



//��EEPROM��ȡ����

uint32_t EEPROM_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead)
{
	uint32_t I2C_TimeOut = I2C_FLAG_LONGTime;
	//������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
	}
	
	//EV5�¼�����⵽�������豸��ַ
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	I2C_TimeOut = I2C_FLAG_LONGTime;
  while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(5);
	}

	//EV6�¼�����⵽������Ҫ�����Ĵ洢��Ԫ��ַ
	I2C_SendData (EEPROM_I2C,addr);
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(6);
	}

	
	//�ڶ�����ʼ�ź�
	//������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
	}
	
	//EV5�¼�����⵽�������豸��ַ
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Receiver);
	I2C_TimeOut = I2C_FLAG_LONGTime;
  while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(5);
	}


	while(numByteToRead)
	{
		if(numByteToRead == 1)
		{		
			//���Ϊ���һ���ֽ�
			I2C_AcknowledgeConfig (EEPROM_I2C,DISABLE);
		}		
		
		I2C_TimeOut = I2C_FLAG_LONGTime;
		//EV7�¼�����⵽	
		while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_RECEIVED ) == ERROR)
		{
			I2C_TimeOut--;
		  if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(8);
		}

		//EV7�¼�����⵽�������ݼĴ������µ���Ч����	
		*data = I2C_ReceiveData(EEPROM_I2C);
		
		data++;
		
		numByteToRead--;
		
	}
	
	
	//���ݴ������
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);	
	
	//��������ACKʹ�ܣ��Ա��´�ͨѶ
	I2C_AcknowledgeConfig (EEPROM_I2C,ENABLE);
  return 0;
}


//�ȴ�EEPROM�ڲ�ʱ�����
uint32_t EEPROM_WaitForWriteEnd(void)
{
	uint32_t I2C_TimeOut = I2C_FLAG_LONGTime;
	uint32_t I2C_Other_TimeOut = I2C_FLAG_LONGTime;
	do
	{
		//������ʼ�ź�
		I2C_GenerateSTART(EEPROM_I2C,ENABLE);

		while(I2C_GetFlagStatus (EEPROM_I2C,I2C_FLAG_SB) == RESET)
		{
			I2C_TimeOut--;
		  if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
		}
		
		//EV5�¼�����⵽�������豸��ַ
		I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
		I2C_Other_TimeOut--;
		if(I2C_Other_TimeOut == 0) return I2C_TIMEOUT_CALL(5);
	}  
	while(I2C_GetFlagStatus (EEPROM_I2C,I2C_FLAG_ADDR) == RESET );

	//EEPROM�ڲ�ʱ����ɴ������
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);
  return 0;	
}


//��ʱ���
static uint32_t I2C_TIMEOUT_CALL(uint8_t fault)
{
	EEPROM_ERROR_FEEDBACK(fault);
	return 0;
}	


