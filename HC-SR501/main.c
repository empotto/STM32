#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"

void exti_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	GPIO_InitTypeDef pin;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	//PB0 pini ayarlamasi
	pin.GPIO_Mode=GPIO_Mode_IN;
	pin.GPIO_PuPd=GPIO_PuPd_NOPULL;
	pin.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOB,&pin);
    //exti ayarlamasi
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource0);//PB0 Exti Kesme
	exti.EXTI_Line=EXTI_Line0;
	exti.EXTI_Mode=EXTI_Mode_Interrupt;
	exti.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd=ENABLE;
	EXTI_Init(&exti);
	//Nvic ayarlari
    nvic.NVIC_IRQChannel=EXTI0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority=0;
    nvic.NVIC_IRQChannelSubPriority=1;
    nvic.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&nvic);
}
void EXTI0_IRQHandler()
{
	EXTI_ClearITPendingBit(EXTI_Line0);
    GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
}
void gpio_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef gpio;

	gpio.GPIO_Pin=GPIO_Pin_15;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init(GPIOD,&gpio);
}
int main(void)
{
  gpio_init();
  exti_init();
    while(1)
    {
    }
}
