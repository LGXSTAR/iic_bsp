#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

#define LED_R_GPIO_PIN              GPIO_Pin_8
#define LED_R_GPIO_PORT             GPIOA
#define LED_R_GPIO_CLK              RCC_APB2Periph_GPIOA

#define LED_G_GPIO_PIN              GPIO_Pin_2
#define LED_G_GPIO_PORT             GPIOD
#define LED_G_GPIO_CLK              RCC_APB2Periph_GPIOD

#define    ON        1
#define    OFF       0

// \  C������������з������治�����κεĶ���

#define   LED_R(a)   if(a) \
	                       GPIO_ResetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN); \
                     else  GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);
#define   LED_G(a)   if(a) \
	                       GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN); \
                     else  GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);

void LED_GPIO_Config(void);

#endif /* __BSP_LED_H */


