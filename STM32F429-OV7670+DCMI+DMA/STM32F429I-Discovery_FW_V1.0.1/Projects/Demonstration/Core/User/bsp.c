/**
  ******************************************************************************
  * @file    bsp.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   bsp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "rtc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TS_Pressed;
__IO uint32_t TS_Orientation = 0;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE            USB_OTG_Core __ALIGN_END;
__ALIGN_BEGIN USBH_HOST                      USB_Host __ALIGN_END;

/* Private function prototypes -----------------------------------------------*/
uint32_t BSP_TSC_Init(void);
void Alarm_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Inititialize the target hardware.
* @param  None
* @retval 0: if all initializations are OK.
*/
uint32_t LowLevel_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure GPIO PC1 to set L3GD20 Chip Select to Reset */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Deselect : Chip Select high */
  GPIO_SetBits(GPIOC, GPIO_Pin_1);
  
  /* Initialize the LEDs */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /*Init Touchscreen */
  BSP_TSC_Init();
  
  /* Initialize the SDRAM */
  SDRAM_Init();
  
  /*Init USB Host */
  USBH_Init(&USB_OTG_Core,
            USB_OTG_HS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb,
            &USBH_USR_cb);
  
  /*Init the RTC */
  RTC_Configuration();
  
  /* Initialize the Alarm */
  Alarm_Init();
  
  /* Enable the CRC Module */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);  
  
  return 0;
}


/**
  * @brief  Initializes the IO Expander registers.
  * @param  None
  * @retval 0: if all initializations are OK.
*/
uint32_t BSP_TSC_Init(void)
{
  __disable_irq();   
  IOE_Config();
  __enable_irq ();
  return 0; /* Configuration is OK */
}

/**
  * @brief  Read the TS data and update the GUI Touch structure
  * @param  None
  * @retval None
  */
void BSP_Pointer_Update(void)
{
  GUI_PID_STATE TS_State;
  TP_STATE  *ts;
  static uint8_t prev_state = 0;
  
  portENTER_CRITICAL();
  ts = IOE_TP_GetState();
    portEXIT_CRITICAL();
  
  if(TS_Orientation == 0)
  {
    TS_State.x = ts->X;
    TS_State.y = ts->Y;
  }
  else
  {
    TS_State.y = ts->X;
    TS_State.x = 320 - ts->Y;
  }
  TS_State.Pressed = (ts->TouchDetected == 0x80);
  
  if(prev_state != TS_State.Pressed )
  {
    prev_state = TS_State.Pressed;
    TS_State.Layer = 1;
    GUI_TOUCH_StoreStateEx(&TS_State); 
  }
}


/**
  * @brief  RTC Alarm A Interrupt Configuration
  * @param  None
  * @retval None
*/
void Alarm_Init(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;
  
  __disable_irq();
  
  /* EXTI configuration */
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  __enable_irq ();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
