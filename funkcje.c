#include "stm32f10x.h"
#include "lcd.h"
#include "funkcje.h"


#define ADC_ADDRESS     ((uint32_t)0x4001224C)
extern __IO uint16_t ADC3ConvertedValue;




void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

void Init_RCC(void){

  
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(/*RCC_APB1Periph_TIM4 |*/ RCC_APB1Periph_TIM3 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
}

  void Init_GPIO(void){
    
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}




void wait_ms(int a){
  int i;
  for(i=0;i<a;i++)
    Delay(239);
  
}
  
void init_lcd(){
    HI(rst);
    wait_ms(1);
    LOW(rst);
    wait_ms(1);
    HI(rst);
    HI(cs);
    HI(rs);
    HI(wr);
    HI(rd);
}

void init_cog(void){
    cog_wr0(0x11);
    wait_ms(10);          //Sleep off
    cog_wr1(0x26,0x04);        //gamma curve
    cog_wr1(0xF2,0x00);        //gamma adjust
    cog_wr0(0x13);            //Normal Display On
    cog_wr0(0x29);            //Display On
    cog_wr1(0x36,0x20);        //Ustawia kolejnoœæ RGB i kolejnosc rysowania pikseli - wczeœniej by³o 0x20 zamiast 0x10
    cog_wr1(0x3A,0x55); 
    cog_wr4(0x2A,0,0,0,159);
    cog_write(white, 20480);
}


void Init_TIM(void) {
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 100000) - 1;     //wartoœæ ustawiaj¹ca  czêstotliwoœæ
  __IO uint16_t CCR1_Val = 28;             
  __IO uint16_t CCR2_Val = 40;

  //ustawienie podstawowych parametrów timera
  TIM_TimeBaseStructure.TIM_Period = 1000-1;           //Okres 20 000 taktów ergo 2 sek przy 10 kHz
  TIM_TimeBaseStructure.TIM_Prescaler = 0;              //ustawiany ni¿ej
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate); //ustawia f = 100 kHz
 
 //ustawienia kana³u 1. 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

  //ustawienia kana³u 2.
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
 
  //w³¹czenie przerwañ i licznika
  TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  
  /*
  //Timer 4 do wyœwietlania wyników co 1 s
  uint16_t PrescalerVal = (uint16_t) ((SystemCoreClock / 2) / 10000) - 1; 
  TIM_TimeBaseStructure.TIM_Period = 1000-1;           //Okres 100 taktów ergo 1 sek przy 100 Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 0;              //ustawiany ni¿ej
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_PrescalerConfig(TIM4,PrescalerVal , TIM_PSCReloadMode_Immediate);
  
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM4, ENABLE);*/
}

void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
   
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /*
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/
}

void Init_DMA(){
  DMA_InitTypeDef       DMA_InitStructure;
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);
}


void Init_ADC(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;  
  ADC_Init(ADC3, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
  
  ADC_DMACmd(ADC3, ENABLE);
  ADC_Cmd(ADC3, ENABLE);
  /*
  ADC_ResetCalibration(ADC3);	                                       
  while(ADC_GetResetCalibrationStatus(ADC3));                         
  ADC_StartCalibration(ADC3);                                        
  while(ADC_GetCalibrationStatus(ADC3));
  */
}

