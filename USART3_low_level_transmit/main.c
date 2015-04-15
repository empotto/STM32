#include "STM32F4xx.h"

///////////////////////////////////////////////////////////////////////////////
//Tx->PD8
//Rx->PD9
///////////////////////////////////////////////////////////////////////////////
#define MAX_STRLEN 10 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string
void init()
{
    RCC->CFGR |= 0x00009400;         // AHB ve APB hizlarini max degerlere set edelim
    RCC->CR |= 0x00010000;            // HSE Xtal osc calismaya baslasin
    while (!(RCC->CR & 0x00020000));// Xtal osc stabil hale gelsin
    RCC->PLLCFGR = 0x07405408;      // PLL katsayilarini M=8, N=336, P=2 ve Q=7 yapalim
//  RCC->PLLCFGR = 0x07402A04;     // PLL katsayilarini M=4, N=168, P=2 ve Q=7 yapalim
    RCC->CR |= 0x01000000;            // PLL calismaya baslasin  (Rehber Sayfa 95)
    while(!(RCC->CR & 0x02000000)); // Pll hazir oluncaya kadar bekle
//  FLASH->ACR = 0x00000705;        // Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
    FLASH->ACR = 0x00000605;        // Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
    RCC->CFGR |= 0x00000002;        // Sistem Clk u PLL uzerinden besleyelim
    while ((RCC->CFGR & 0x0000000F) != 0x0000000A); // Besleninceye kadar bekle
    RCC->AHB1ENR |= 0x0000000F;    // GPIO A,B,C,D clock'u aktif edelim
    GPIOD->MODER = 0x55000000;     // GPIOD nin 15, 14, 13, 12 pinleri cikis tanimlandi (LEDler icin)
    GPIOD->OSPEEDR= 0xFF000000;   // GPIOD nin 15,14,13,12 cikislari en yuksek hizda kullanacagiz

    RCC->APB1ENR|=0x00040000;  //USART 3 CLOCK AKTIF
}
//////////////////////////USART INIT//////////////////////////7
void usart_init()
{
	GPIOD->MODER=0x000A0000; //Pd8 VE Pd9 ALTERNATIF FONK OLARAK TANIMLANDI
	GPIOD->AFR[1]=0x00000077;   //PB8 VE PB9 USART PÝNLERÝ OLARAK ATANDI(AFRH)
	USART3->BRR=0X1112;            // 9600 Baud
	USART3->CR1|=0x00000008;    //TRANSMITTER ENABLE(SADECE GONDERME YAPILACAK)
	//USART3->CR1|=0x00000004;    //Receiver enable
	USART3->CR2|=0X00000000;    //STOP BÝT SAYÝSÝ 13-12 BÝTLERÝ ÝLE AYARLANIYOR
	//USART3->CR1|=0x00000020;    //Rx interrupt enable
	//NVIC->ISER[1]|=0x80;       //Usart3 rx interruptýna izin veriliyor
	USART3->CR1|=0x00002000;     //USART3 ENABLE

}
void send_char(char *c)
{
	while(!(USART3->SR&0x00000040));      // TX Buffer dolu ise bekle (Rehber Sayfa 646)
	   USART3->DR=c;
}

void print(char *s)
  {

     while(*s)
       {
    	 while(!(USART3->SR&0x00000040));
    	 USART3->DR=s;
        *s++;
       }
  }
void delay()
{
unsigned int i;

     for(i=0;i<0x900000;i++);
}

int main()
{
	init();
    usart_init();
    print("USART3 DENEMESI\n");
	while(1)
	{

	}
}
