#include "stm32f4xx.h"
void Init()
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
    GPIOD->OSPEEDR= 0xFFFFFFFF;   // GPIOD nin tum cikislari en yuksek hizda kullanacagiz

}
///////////////ADC MODULU AYARLARI/////////////////////////////
void ADC_init(void)
    {
	GPIOC->MODER=0x00000C00;           //c5 ANALOG mode YAPILDI
	RCC->APB2ENR|=0x00000100;          //ADC1 MODULU CLOCK AKTIF
	ADC1->CR1|=0x01000000;             //10 BÝT CÖZÜNÜRLÜK SECÝLDÝ
	ADC1->SMPR2=0x00000007;            //EN DUSUK CEVRIM HIZI SECILDI
	ADC1->SQR1=0x00000000;             //1 KANAL CEVRÝME GÝRECEK
	ADC1->SQR3 |= 0x0000000F;          //ÝLK PC5 GÝRECEK CEVRÝME
	ADC->CCR|=0x00030000;              //CLOCK 8 E BOLUNDU
	ADC1->CR2|=0x00000001;             //ADON BÝTÝ SET EDÝLDÝ(ADC YÝ AKTÝF ETTÝK)

    }
////////////////////////////////////////////////////////////////

/////////////////////ADC OKUMA /////////////////////////////////
uint16_t adc_oku(void)
{
  ADC1->CR2|=0x40000000;         //ADC aktif
  uint16_t deger=0;
  while(!(ADC1->SR & 0x00000002)); //Cevrim bitene kadar bekle
  deger=ADC1->DR;
  return deger;
}
/////////////////////////////////////////////////////////////////
void delay()
  {
    unsigned int i=0;
	for(i=0;i<0x90000000;i++);
  }
int main()
{
	 Init();
     ADC_init();
	 uint16_t  sonuc;
     float voltaj;
	 while(1)
	  {
		sonuc=adc_oku();
		voltaj=sonuc*0.00283203125; //    VDD/1024 = 2.9/1024
         delay();
       }
}




