// includes --------------------------------------------------------------------
#include "main.h"
#include "stlogo.h"
#include "usb_audio.h"
#include "usart.h"

#include "user_lcd.h"

// private variables -----------------------------------------------------------
USBD_HandleTypeDef USBD_Device;
// static int32_t pending_buffer   = -1;
// static int32_t active_area      = INVALID_AREA;
// static uint32_t ImageIndex      = 0;
uint32_t watchdogTimer    = 0;
uint32_t watchdogCounter  = 0;
uint32_t serialTimer 	  = 0;
uint32_t touchscreenTimer = 0;
bool shouldPrintSamples   = false;
float in_z1               = 0;
float in_z2               = 0;
float out_z1              = 0;
float out_z2              = 0;

uint8_t pColLeft[]        = {0x00, 0x00, 0x01, 0x8F}; // 0 -> 399
uint8_t pColRight[]       = {0x01, 0x90, 0x03, 0x1F}; // 400 -> 799
uint8_t pPage[]           = {0x00, 0x00, 0x01, 0xDF}; // 0 -> 479
uint8_t pSyncLeft[]       = {0x02, 0x15};             // Scan @ 533

uint32_t xDebug[100];

#if USE_AUDIO_TIMER_VOLUME_CTRL
  TIM_HandleTypeDef TimHandle;
  // Prescaler declaration
  uint32_t uwPrescalerValue = 0;
#endif // USE_AUDIO_TIMER_VOLUME_CTRL

// private function prototypes -------------------------------------------------
// static void OnError_Handler(uint32_t condition);
static void     SystemClock_Config(void);
static void     CPU_CACHE_Enable(void);
static void     USB_Init(void);

#if USE_AUDIO_TIMER_VOLUME_CTRL
  static HAL_StatusTypeDef Timer_Init(void);
#endif // USE_AUDIO_TIMER_VOLUME_CTRL

// external variables ----------------------------------------------------------
extern  USBD_AUDIO_InterfaceCallbacksfTypeDef audio_class_interface;

// private functions -----------------------------------------------------------

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
    memset(xDebug, 0, sizeof xDebug);

    CPU_CACHE_Enable();

    /* STM32F7xx HAL library initialization:
     - Configures the Flash ART accelerator on ITCM interface
     - Configures the Systick to generate an interrupt each 1 msec
     - Sets NVIC Group Priority to 4
     - Low Level Initialization
  */
    HAL_Init();

    // configures the system clock to have a frequency of 200 MHz
    SystemClock_Config();

    BSP_SDRAM_Init();
    USB_Init();

    HAL_Delay(1000);

    USART1_UART_Init();
    LCD_Init();
    Touchscreen_Init();


    uint8_t initString[] = "\r\n--- Horoscope Initialization Complete! ---\r\n";
    HAL_UART_Transmit(&UART1_Handle, initString, sizeof(initString), 10);

  #if USE_AUDIO_TIMER_VOLUME_CTRL
    // configures timer for volume control handling 
    Timer_Init();
  #endif // USE_AUDIO_TIMER_VOLUME_CTRL 

    int32_t messageCounter = 0;
    uint8_t messageCounterString[100];

    // Infinite loop 
    while (1)
    {
        if(++touchscreenTimer > 10)
        {
            Touchscreen_ButtonHandler();
            touchscreenTimer = 0;
        }

        if(shouldPrintSamples)
        {
            LCD_PrintDebugVariable(20, true);
            shouldPrintSamples = false;
        }

        if(++serialTimer > 1000)
        {
          uint8_t stringSize = sprintf((char*)messageCounterString, "Horoscope [architecture-fix] with BSP via Serial (%i)\r\n", (int)messageCounter);
          HAL_UART_Transmit(&UART1_Handle, messageCounterString, stringSize, 10);
          messageCounter++;
          serialTimer = 0;
        }

        if(++watchdogTimer > 10)
        {	
            LCD_UpdateWatchdog(&watchdogCounter);
            watchdogTimer = 0;
        }
    }
}

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void)
{
    SCB_EnableICache();
    SCB_EnableDCache();
}


#if USE_AUDIO_TIMER_VOLUME_CTRL

/**
 * @brief  This function initialize timer to handle volume control.
 * @param  None
 * @retval Status
 */
static HAL_StatusTypeDef Timer_Init(void)
{
    //##-1- Configure the TIM peripheral #######################################
    /* -----------------------------------------------------------------------
       In this example TIM3 input clock (TIM3CLK)  is set to APB1 clock (PCLK1) x2,
       since APB1 prescaler is equal to 4.
        TIM3CLK = PCLK1*2
        PCLK1 = HCLK/4
        => TIM3CLK = HCLK/2 = SystemCoreClock/2
       To get TIM3 counter clock at 10 KHz, the Prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock/2) /10 KHz) - 1

       Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
       Each time the core clock (HCLK) changes, user had to update SystemCoreClock
       variable value. Otherwise, any configuration based on this variable will be incorrect.
       This variable is updated in three ways:
        1) by calling CMSIS function SystemCoreClockUpdate()
        2) by calling HAL API function HAL_RCC_GetSysClockFreq()
        3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
       ----------------------------------------------------------------------- */

    // Compute the prescaler value to have TIM_VolumeChange counter clock equal to 10000 Hz 
    uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / 10000) - 1;

    // Set TIM_VolumeChange instance 
    TimHandle.Instance = TIM_VolumeChange;

    /* Initialize TIM_VolumeChange peripheral as follows:
     + Period = 1000 - 1
     + Prescaler = ((SystemCoreClock / 2)/10000) - 1
     + ClockDivision = 0
     + Counter direction = Up
     */
    TimHandle.Init.Period            = 1000 - 1;
    TimHandle.Init.Prescaler         = uwPrescalerValue;
    TimHandle.Init.ClockDivision     = 0;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;

    if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
    {
        // Initialization Error 
        Error_Handler();
    }

    //##-2- Start the TIM Base generation in interrupt mode ####################
    // Start Channel1 
    if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    {
        // Starting Error 
        Error_Handler();
    }
    return HAL_OK;
}
#endif // USE_AUDIO_TIMER_VOLUME_CTRL 

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 192000000
 *            HCLK(Hz)                       = 192000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 25000000
 *            PLL_M                          = 25
 *            PLL_N                          = 384
 *            PLL_P                          = 2
 *            PLL_Q                          = 8
 *            PLL_R                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 6
 * @param  None
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    // Enable HSE Oscillator and activate PLL with HSE as source 
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 8;
    RCC_OscInitStruct.PLL.PLLR = 7;

    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();

    // Select PLLSAI output as USB clock source 
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct)  != HAL_OK)
        Error_Handler();

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers 
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
        Error_Handler();
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
    // User may add here some code to deal with this error 
    while(1)
    {
    }
}
void USBD_error_handler(void)
{
    Error_Handler();
}

///**
// * @brief  On Error Handler on condition TRUE.
// * @param  condition : Can be TRUE or FALSE
// * @retval None
// */
//static void OnError_Handler(uint32_t condition)
//{
//	if(condition)
//	{
//		BSP_LED_On(LED1);
//		while(1); // blocking on error
//	}
//}

static void USB_Init(void)
{
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    // Init Device Library 
    BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"USB Device Library ..... ", LEFT_MODE);
    USBD_Init(&USBD_Device, &AUDIO_Desc, 0);
    BSP_LCD_DisplayStringAt(420, 10, (uint8_t *)"OK", LEFT_MODE);

    // Add Supported Class 
    BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"USB Register Class ..... ", LEFT_MODE);
    USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);
    BSP_LCD_DisplayStringAt(420, 30, (uint8_t *)"OK", LEFT_MODE);

    // Add Interface callbacks for AUDIO Class 
    BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"USB Register Interface . ", LEFT_MODE);
    USBD_AUDIO_RegisterInterface(&USBD_Device, &audio_class_interface);
    BSP_LCD_DisplayStringAt(420, 50, (uint8_t *)"OK", LEFT_MODE);

    // Start Device Process 
    BSP_LCD_DisplayStringAt(0, 70, (uint8_t *)"USB Init ............... ", LEFT_MODE);
    USBD_Start(&USBD_Device);
    BSP_LCD_DisplayStringAt(420, 70, (uint8_t *)"OK", LEFT_MODE);

}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    // Infinite loop 
    while (1)
    {
    }
}
#endif

//*********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE***
