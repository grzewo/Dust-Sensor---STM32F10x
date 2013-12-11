#include "stm32f10x.h"
#include "init.h"
#include "lcd.h"


void RCC_Config(void)
{
  ErrorStatus HSEStartUpStatus;                         

  RCC_DeInit();                                          
  RCC_HSEConfig(RCC_HSE_ON);                             
  HSEStartUpStatus = RCC_WaitForHSEStartUp();            
  if(HSEStartUpStatus == SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);                  
                                                         
    RCC_HCLKConfig(RCC_SYSCLK_Div1);                     
    RCC_PCLK2Config(RCC_HCLK_Div1);                      
    RCC_PCLK1Config(RCC_HCLK_Div2);                      
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
                                                         
                                                           
                                                           
    RCC_PLLCmd(ENABLE);                                  
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);  
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);           
    while(RCC_GetSYSCLKSource() != 0x08);                

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC2, ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2);
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  } else {
  }
}


void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
//Konfigurowanie kontrolera przerwan NVIC
#ifdef  VECT_TAB_RAM
  // Jezeli tablica wektorow w RAM, to ustaw jej adres na 0x20000000
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  // VECT_TAB_FLASH
  // W przeciwnym wypadku ustaw na 0x08000000
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

  	//przerwanie UP (przepelnienie) timera1
        NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    
      	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}


void GPIO_Config(void)
{
  //konfigurowanie portow GPIO
  GPIO_InitTypeDef  GPIO_InitStructure;

  /*Tu nalezy umiescic kod zwiazany z konfiguracja poszczegolnych portow GPIO potrzebnych w programie*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
}


void Init_LCD(){
    HI(rst);
    Delay(100);
    LOW(rst);
    Delay(100);
    HI(rst);
    HI(cs);
    HI(rs);
    HI(wr);
    HI(rd);
}

void Init_COG(void){
    cog_wr0(0x11);
    Delay(100);          //Sleep off
    cog_wr1(0x26,0x04);        //gamma curve
    cog_wr1(0xF2,0x00);        //gamma adjust
    cog_wr0(0x13);            //Normal Display On
    cog_wr0(0x29);            //Display On
    cog_wr1(0x36,0x20);        //Ustawia kolejnoœæ RGB i kolejnosc rysowania pikseli - wczeœniej by³o 0x20 zamiast 0x10
    cog_wr1(0x3A,0x55); 
    cog_wr4(0x2A,0,0,0,159);
    cog_write(green, 20480);
}


void Delay(uint32_t nCount)
{
  while(nCount--)
  {
  }
}

void Init_TIM(void) {

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1;                //taktowanie licznka fclk = 72MHz/720 = 100kHz
	TIM_TimeBaseStructure.TIM_Period = 1000-1;                    //okres przepelnien licznika = 1000 taktow = 10 milisekund
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //dzielnik zegara dla ukladu generacji dead-time i filtra
 	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;               //licznik powtorzen
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //tryb pracy licznika

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);              //Inicjalizacja licznika
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;          //tryb pracy kanalu
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//wlaczenie generowania sygnalu na wyjsciu licznika
	TIM_OCInitStructure.TIM_Pulse =50;                         //5000 taktow = 0.5s od restartu licznika
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //polaryzacja wyjscia

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);                     //Inicjalizacja kanalu 1 licznika TIM1
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
      
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 80;
        TIM_OC2Init(TIM1, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);  
        
        TIM_ITConfig(TIM1, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
        TIM_Cmd(TIM1, ENABLE);
}

void Init_ADC(void)
{
  //konfigurowanie przetwornika AC
  ADC_InitTypeDef ADC_InitStructure;

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //Jeden przetwornik, praca niezalezna
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //Pomiar jednego kanalu, skanowanie kanalow nie potrzebne
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //Pomiar w trybie jednokrotnym
        //ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                //Pomiar w trybie ciaglym
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//Brak wyzwalania zewnetrznego
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;            	//Wyrownanie danych do prawej - 12 mlodszych bitow znaczacych
	ADC_InitStructure.ADC_NbrOfChannel = 1; 	                          //Liczba uzywanych kanalow =1
	ADC_Init(ADC2, &ADC_InitStructure);                                 //Incjalizacja przetwornika

	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_1Cycles5);
	                                                                    //Kanal 14 - GPIOC4 - potencjometr na plytce ZL27ARM
                                                                      //Grupa podstawowa, czas probkowania 1,5+12,5=14 cykli ->  f probkowania = 14MHz/14cykli = 1MHz

	ADC_Cmd(ADC2, ENABLE);						                                  //Wlacz ADC1

	ADC_ResetCalibration(ADC2);	                                        //Reset rejestrow kalibracyjnych ADC1
	while(ADC_GetResetCalibrationStatus(ADC2));                         //Odczekanie na wykonanie resetu
	ADC_StartCalibration(ADC2);                                         //Kalibracja ADC1
	while(ADC_GetCalibrationStatus(ADC2));                              //Odczekanie na zakonczenie kalibracji ADC1

}