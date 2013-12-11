#include "stm32f10x.h"
#include "init.h"
#include "lcd.h"

uint16_t ADCConvertedValue;
uint32_t ADCConvertedVoltage;

void main(void)
{
    RCC_Config();
    GPIO_Config();
    NVIC_Config();
    
  Init_LCD();
  Delay(10000);          //Daj mu czas na start po resecie
  Init_COG();
    
  cog_write_string("Grzegorz",10,5);
  cog_write_string("jest",10,25);
  cog_write_string("miszczem",10,45);
  cog_write_string("esteemow 32",10,65);
    
    Init_TIM();
    Init_ADC();
    

    



  while(1)
  {
    uint32_t i = ADCConvertedVoltage;
     cog_write_number(i%10,105,90);
     i/=10;
     cog_write_number(i%10,90,90);
     i/=10;
     cog_write_number(i%10,75,90);
     i/=10;
     cog_write_number(i%10,60,90);

  }
}
