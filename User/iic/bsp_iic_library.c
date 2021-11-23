#include "bsp_iic_library.h"

static uint32_t I2C_TIMEOUT_CALL(uint8_t fault);

static void Change_I2C_PIN_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
		//GPIOC_Pin12配置为开漏输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_12);

    //GPIOC_Pin11配置为开漏输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_11);
}

//void I2C_Pin_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	// 打开I2C_GPIO外设的时钟
//	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_SCL_GPIO_CLK|EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
//	//SCL GPIO配置为开漏复用模式
//	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

//  //SDA GPIO配置为开漏复用模式
//	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_GPIO_PIN;
//	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
//}

//void I2C_Config(void)
//{
//	I2C_InitTypeDef  I2C_InitStructure;
//	
//	// 打开I2C1的时钟
//	EEPROM_I2C_APBxClkCmd(EEPROM_I2C_CLK, ENABLE);

//	// 配置IIC的工作参数
//	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;   //使能应答
//	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //使用7位地址模式
//	I2C_InitStructure.I2C_ClockSpeed = EEPROM_I2C_BAUDRATE;   //配置时钟SCL频率
//	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  //高电平数据稳定，低电平数据变化 SCL 时钟线的占空比
//	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;  
//	I2C_InitStructure.I2C_OwnAddress1 = STM32_I2C_OWN_ADDR;
//	
//	I2C_Init(EEPROM_I2C,&I2C_InitStructure);   //STM32 IIC自身设备地址   只要是总线上的唯一地址即可
//	
//	I2C_Cmd(EEPROM_I2C,ENABLE);
//}


void I2C_EE_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;
	Change_I2C_PIN_Config();
 	
	// 打开I2C_GPIO外设的时钟
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_SCL_GPIO_CLK|EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
	// 打开I2C1的时钟
	EEPROM_I2C_APBxClkCmd(EEPROM_I2C_CLK, ENABLE);
	
	//SCL GPIO配置为开漏复用模式
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  //SDA GPIO配置为开漏复用模式
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_GPIO_PIN;
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);	
	
	// 配置IIC的工作参数
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;   //使能应答
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //使用7位地址模式
	I2C_InitStructure.I2C_ClockSpeed = EEPROM_I2C_BAUDRATE;   //配置时钟SCL频率
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  //高电平数据稳定，低电平数据变化 SCL 时钟线的占空比
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;  
	I2C_InitStructure.I2C_OwnAddress1 = STM32_I2C_OWN_ADDR;
	
	I2C_Init(EEPROM_I2C,&I2C_InitStructure);   //STM32 IIC自身设备地址   只要是总线上的唯一地址即可
	
	I2C_Cmd(EEPROM_I2C,ENABLE);
}

//向EEPROM写入一个字节

uint32_t EEPROM_Byte_Write(uint8_t addr,uint8_t data)
{
	uint32_t I2C_TimeOut = I2C_FLAG_LONGTime;
	//产生起始信号
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
	}
	
	//EV5事件被检测到，发送设备地址
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	
	I2C_TimeOut = I2C_FLAG_LONGTime;
  while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(5);
	}

	//EV6事件被检测到，发送要操作的存储单元地址
	I2C_SendData (EEPROM_I2C,addr);
	
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(6);
	}

  //EV8事件被检测到，发送要存储的数据
	I2C_SendData (EEPROM_I2C,data);
	
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(7);
	}
	
	//数据传输完成
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);	
  return 0;
}



//向EEPROM写入多个字节（页写入），每次写入不能超过8个字节

uint32_t EEPROM_Page_Write(uint8_t addr,uint8_t *data,uint8_t numByteToWrite)
{
	uint32_t I2C_TimeOut = I2C_FLAG_LONGTime;
	//产生起始信号
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
	}
	
	//EV5事件被检测到，发送设备地址
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	
	I2C_TimeOut = I2C_FLAG_LONGTime;
  while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(5);
	}
	
	//EV6事件被检测到，发送要操作的存储单元地址
	I2C_SendData (EEPROM_I2C,addr);
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(6);
	}

	
	while(numByteToWrite)
	{
		//EV8事件被检测到，发送要存储的数据
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
	//数据传输完成
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);	
  return 0;
}



//从EEPROM读取数据

uint32_t EEPROM_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead)
{
	uint32_t I2C_TimeOut = I2C_FLAG_LONGTime;
	//产生起始信号
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
	}
	
	//EV5事件被检测到，发送设备地址
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	I2C_TimeOut = I2C_FLAG_LONGTime;
  while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(5);
	}

	//EV6事件被检测到，发送要操作的存储单元地址
	I2C_SendData (EEPROM_I2C,addr);
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING ) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(6);
	}

	
	//第二次起始信号
	//产生起始信号
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	I2C_TimeOut = I2C_FLAG_LONGTime;
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		I2C_TimeOut--;
		if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
	}
	
	//EV5事件被检测到，发送设备地址
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
			//如果为最后一个字节
			I2C_AcknowledgeConfig (EEPROM_I2C,DISABLE);
		}		
		
		I2C_TimeOut = I2C_FLAG_LONGTime;
		//EV7事件被检测到	
		while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_RECEIVED ) == ERROR)
		{
			I2C_TimeOut--;
		  if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(8);
		}

		//EV7事件被检测到，即数据寄存器有新的有效数据	
		*data = I2C_ReceiveData(EEPROM_I2C);
		
		data++;
		
		numByteToRead--;
		
	}
	
	
	//数据传输完成
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);	
	
	//重新配置ACK使能，以便下次通讯
	I2C_AcknowledgeConfig (EEPROM_I2C,ENABLE);
  return 0;
}


//等待EEPROM内部时序完成
uint32_t EEPROM_WaitForWriteEnd(void)
{
	uint32_t I2C_TimeOut = I2C_FLAG_LONGTime;
	uint32_t I2C_Other_TimeOut = I2C_FLAG_LONGTime;
	do
	{
		//产生起始信号
		I2C_GenerateSTART(EEPROM_I2C,ENABLE);

		while(I2C_GetFlagStatus (EEPROM_I2C,I2C_FLAG_SB) == RESET)
		{
			I2C_TimeOut--;
		  if(I2C_TimeOut == 0) return I2C_TIMEOUT_CALL(4);
		}
		
		//EV5事件被检测到，发送设备地址
		I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
		I2C_Other_TimeOut--;
		if(I2C_Other_TimeOut == 0) return I2C_TIMEOUT_CALL(5);
	}  
	while(I2C_GetFlagStatus (EEPROM_I2C,I2C_FLAG_ADDR) == RESET );

	//EEPROM内部时序完成传输完成
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);
  return 0;	
}


//超时检测
static uint32_t I2C_TIMEOUT_CALL(uint8_t fault)
{
	EEPROM_ERROR_FEEDBACK(fault);
	return 0;
}	


