
#include "stm32f10x_it.h"

extern uint16_t ADCConvertedValue;
extern uint32_t ADCConvertedVoltage;

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

void TIM1_UP_IRQHandler(void)
{      
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)	{   //przeladowanie licznika
  	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
  	GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);  	//LED1
  }
}


void TIM1_CC_IRQHandler(void){
  
if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);		               //wyzwolenie pojedynczego pomiaru
      while (!ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC));           //odczekaj na zakonczenie konwersji
      ADCConvertedValue = ADC_GetConversionValue(ADC2);             //pobiez zmierzona wartosc
      ADCConvertedVoltage =  ADCConvertedValue * 8059/10000;
    
 }
  
  else if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
    
  }
}


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
