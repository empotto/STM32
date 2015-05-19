#ifndef _DELAY_H
#define _DELAY_H


#include "stm32f4xx.h"
uint32_t sayac;
void delay_ms(uint32_t delay)
{
SysTick_Config(SystemCoreClock/1000);// 1 ms
sayac=0;
while(sayac!=delay);

}

void SysTick_Handler(void)
{
sayac++;
}
void delay_us(uint32_t delay)
{
	SysTick_Config(SystemCoreClock/1000000);//1 us elde etmek icin
	sayac=0;
	while(sayac!= delay);

}
#endif
