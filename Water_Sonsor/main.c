#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "delay.h"
#include "stdio.h"
/*
 * Sensor         STM32F4
 * -VCC            -VCC
 * -GND            -GND
 * -D0             -PA0
 * -A0             -PC1
 */
void adc_init()
 {
  GPIO_InitTypeDef analog_pin;
  ADC_InitTypeDef adc_ayar;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);

  analog_pin.GPIO_Mode=GPIO_Mode_AN;
  analog_pin.GPIO_PuPd=GPIO_PuPd_NOPULL;
  analog_pin.GPIO_Pin=GPIO_Pin_1;
  GPIO_Init(GPIOC,&analog_pin);

  ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div8;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_20Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
  ADC_CommonInit(&ADC_CommonInitStructure);

  adc_ayar.ADC_ContinuousConvMode=DISABLE;
  adc_ayar.ADC_ScanConvMode=DISABLE;
  adc_ayar.ADC_DataAlign=ADC_DataAlign_Right;
  adc_ayar.ADC_NbrOfConversion=1;
  adc_ayar.ADC_Resolution=ADC_Resolution_12b;
  ADC_Init(ADC3,&adc_ayar);
  ADC_Cmd(ADC3,ENABLE);
 }

uint16_t adc_oku()
 {
	ADC_RegularChannelConfig(ADC3,ADC_Channel_11,1,ADC_SampleTime_112Cycles);
	ADC_SoftwareStartConv(ADC3);
	while(ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC)==RESET);
	return  ADC_GetConversionValue(ADC3);
 }

void usart_init()
 {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

  GPIO_InitTypeDef pin_ayar;
  USART_InitTypeDef usart;
  usart.USART_BaudRate=9600;
  usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  usart.USART_Mode=USART_Mode_Tx;
  usart.USART_Parity=USART_Parity_No;
  usart.USART_StopBits=USART_StopBits_1;
  usart.USART_WordLength=USART_WordLength_8b;
  USART_Init(USART3,&usart);
  USART_Cmd(USART3,ENABLE);

  pin_ayar.GPIO_Mode=GPIO_Mode_AF;
  pin_ayar.GPIO_Pin=GPIO_Pin_8;
  pin_ayar.GPIO_Speed=GPIO_Speed_50MHz;
  pin_ayar.GPIO_OType = GPIO_OType_PP;
  pin_ayar.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD,&pin_ayar);

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);//D8 pini tx olarak atandi
 }
void USART_puts(USART_TypeDef* USARTx, volatile char *s)
{
  while(*s)
  {
   while( !(USARTx->SR & 0x00000040) );
   USART_SendData(USARTx,*s);
   *s++;
  }
}
void gpio_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef gpio;

	gpio.GPIO_Pin=GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init(GPIOD,&gpio);


	gpio.GPIO_Pin=GPIO_Pin_0;
	gpio.GPIO_Mode=GPIO_Mode_IN;
	gpio.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init(GPIOA,&gpio);
}
int main(void)
{
  usart_init();
  adc_init();
  gpio_init();
  char buffer[20];
    while(1)
    {
    	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
    	{
    	  GPIO_SetBits(GPIOD,GPIO_Pin_12);
    	}
    	else
    	{
    		GPIO_ResetBits(GPIOD,GPIO_Pin_12);
    	}
    	sprintf(buffer,"adc:%d\n",adc_oku());
    	USART_puts(USART3,buffer);
    	GPIO_SetBits(GPIOD,GPIO_Pin_15);
    	delay_ms(500);
    	GPIO_ResetBits(GPIOD,GPIO_Pin_15);
    	delay_ms(500);
    }
}
