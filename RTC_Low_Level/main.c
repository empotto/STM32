#include"stm32f4xx.h"
typedef struct
  {
   uint32_t saat;
   uint32_t dakika;
   uint32_t saniye;
  }saat;

void rtc_init()
 {
   PWR->CR|=0x00000100;
   RCC->BDCR|=0x00008000; //RTC Clock enable
   RCC->BDCR|=0x00000300;//RTC clock hse secildi
   RCC->CFGR|=0x00080000; //  RTCCLOCK=HSE/8= 1 MHZ
   RTC->WPR=0xCA;
   RTC->WPR=0x53;
   RTC->ISR|=0x00000080;//Init biti set edildi.Init
   while(!(RTC->ISR & 0x00000040));//Senkron olmasini bekle
   RTC->PRER|=0x00001F3F;  //7999
   RTC->PRER|=0x007C0000;  //124
   RTC->TR|=0x00400000;   //24 saat formati
   RTC->TR|=0x00212121;  //21:21:21 ayarlandi
   RTC->DR|=0x00213221;  //21/12/2012 ayarlandi
   RTC->CR|=0x00000000;  //24 saat formati 6. bit
   RTC->ISR&=0xFFFFFF7F;    //init
   RTC->WPR=0xFF;
 }

saat saat_oku()
{   uint32_t saat_birler;
    uint32_t saat_onlar;
    uint32_t dakika_birler;
    uint32_t dakika_onlar;
    uint32_t saniye_birler;
    uint32_t saniye_onlar;
    saat zaman;
    uint32_t saat;
    uint32_t dakika;
    uint32_t saniye;
	uint32_t veri=RTC->TR;
	saat_onlar=veri>>20;
	saat_onlar&=0x00000003;
	saat_onlar=saat_onlar*10;
	saat_birler=veri>>16;
	saat_birler&=0x000000F;
	saat=saat_onlar+saat_birler;
    zaman.saat=saat;
    dakika_onlar=veri>>12;
    dakika_onlar&=0x00000007;
    dakika_onlar=dakika_onlar*10;
    dakika_birler=veri>>8;
    dakika_birler&=0x0000000F;
    dakika=dakika_onlar+dakika_birler;
    zaman.dakika=dakika;
    saniye_onlar=veri>>4;
    saniye_onlar&=0x00000007;
    saniye_onlar=saniye_onlar*10;
    saniye_birler=veri & 0x0000000F;
    saniye=saniye_onlar+saniye_birler;
    zaman.saniye=saniye;
return zaman;
}

int main(void)
{   saat vakit;
  uint32_t a;
  uint32_t b;
  uint32_t c;
    rtc_init();
    while(1)
    {
    	vakit=saat_oku();

    	     a=vakit.saat;
    	     b=vakit.dakika;
    	     c=vakit.saniye;

    }
}
