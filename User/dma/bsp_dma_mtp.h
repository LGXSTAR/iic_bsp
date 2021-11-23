#ifndef _BSP_DMA_MTP_H
#define _BSP_DMA_MTP_H

#include "stm32f10x.h"
#include <stdio.h>

// 串口1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler


//外设寄存器基地址
#define USART_DR_ADDRESS         (USART1_BASE + 0x04)

//DMA挂载在总线的时钟
#define USART_TX_DMA_CLK         RCC_AHBPeriph_DMA1

//数据长度
#define SENDBUFF_SIZE            10000
//DMA通道
#define USART_TX_CHANNEL         DMA1_Channel4
//发送完成标志位
#define USART_TX_DMA_FLAG_TC     DMA1_FLAG_TC2


void USARTx_DMA_Config(void);
void USART_Config(void);

#endif /*_BSP_DMA_MTP_H*/
