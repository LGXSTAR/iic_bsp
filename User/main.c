#include "stm32f10x.h"   
#include "bsp_led.h"
#include "bsp_dma_mtp.h"
#include "bsp_systick.h"
#include "bsp_iic_library.h"


#define GPIOA_ODR_Addr      (GPIOA_BASE+0x0C)
#define PAout(n)           *(unsigned int*)((GPIOA_ODR_Addr & 0xF0000000) + 0x02000000 + ((GPIOA_ODR_Addr & 0x00FFFFFF)<<5) + (n<<2))

extern uint8_t SendBuff[SENDBUFF_SIZE];

uint8_t readDat[10]={0};
uint8_t WriteDat[8]={4,5,6,7,8,9,10,18};
int main(void)
{
// 来到这里的时候，系统的时钟已经被配置成72M
//	uint16_t i=0;
	uint8_t n=0;
	LED_GPIO_Config();
//	USARTx_DMA_Config();
	USART_Config();
	SysTick_Init();
	I2C_EE_Config();

	printf("\r\n这是一个IIC通讯实验\r\n");
	
	//EEPROM_Byte_Write(8,0x49);
	//等待写入操作完成
	//EEPROM_WaitForWriteEnd();
	
	//addr%8=0,即为地址对齐
	EEPROM_Page_Write(0,WriteDat,8);
	//等待写入操作完成
	//EEPROM_WaitForWriteEnd();
	
	//EEPROM_Read(0,readDat,8);
	printf("\r\n接收到的数据为:");
	for(n=0;n<10;n++)
	{
	  printf("%d ",readDat[n]);
	}
	printf("\r\n");
	
//	for(i=0;i<SENDBUFF_SIZE;i++)
//	{
//		SendBuff[i] = 'F';
//	}
//	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
	
	while(1)
	{
		//GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
		PAout(8) = 1;
		Delay_ms(500);
		//GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
		PAout(8) = 0;
		Delay_ms(500);
	}
}


