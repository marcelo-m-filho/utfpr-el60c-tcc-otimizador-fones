/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "utfprlogo.h"
#include "usb_audio.h"
#include "image_320x240_argb8888.h"
#include "life_augmented_argb8888.h"
/* Private typedef -----------------------------------------------------------*/
extern LTDC_HandleTypeDef hltdc_discovery;
static DMA2D_HandleTypeDef hdma2d;
extern DSI_HandleTypeDef hdsi_discovery;
DSI_VidCfgTypeDef hdsivideo_handle;
DSI_CmdCfgTypeDef CmdCfg;
DSI_LPCmdTypeDef LPCmd;
DSI_PLLInitTypeDef dsiPllInit;
static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
/* Private define ------------------------------------------------------------*/
#define VSYNC           1
#define VBP             1
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define HACT            400      /* !!!! SCREEN DIVIDED INTO 2 AREAS !!!! */

#define LAYER0_ADDRESS  (LCD_FB_START_ADDRESS)

#define INVALID_AREA      0
#define LEFT_AREA         1
#define RIGHT_AREA        2
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
USBD_HandleTypeDef USBD_Device;
static int32_t pending_buffer = -1;
static int32_t active_area = INVALID_AREA;
static uint32_t ImageIndex = 0;
static const uint32_t * Images[] =
{
	image_320x240_argb8888,
	life_augmented_argb8888,
};

uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
uint8_t pSyncLeft[]   = {0x02, 0x15};             /* Scan @ 533 */

#if USE_AUDIO_TIMER_VOLUME_CTRL
TIM_HandleTypeDef TimHandle;
/* Prescaler declaration */
uint32_t uwPrescalerValue = 0;
#endif /* USE_AUDIO_TIMER_VOLUME_CTRL */
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_DemoDescription(void);
static void OnError_Handler(uint32_t condition);
static void CPU_CACHE_Enable(void);
static uint8_t LCD_Init(void);
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
void LTDC_Init(void);
static void LCD_BriefDisplay(void);
static void CopyPicture(uint32_t *pSrc,
                        uint32_t *pDst,
                        uint16_t x,
                        uint16_t y,
                        uint16_t xsize,
                        uint16_t ysize);

#if USE_AUDIO_TIMER_VOLUME_CTRL
static HAL_StatusTypeDef Timer_Init(void);
#endif /* USE_AUDIO_TIMER_VOLUME_CTRL */
/* externals  variables -----------------------------------------------*/
extern USBD_AUDIO_InterfaceCallbacksfTypeDef audio_class_interface;

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	uint8_t lcd_status = LCD_OK;

	// enables the CPU cache
	CPU_CACHE_Enable();

	/* STM32F7xx HAL library initialization:
	     - Configure the Flash ART accelerator on ITCM interface
	     - Configure the Systick to generate an interrupt each 1 msec
	     - Set NVIC Group Priority to 4
	     - Low Level Initialization
	 */
	// TODO: validate if everything needed is here
	HAL_Init();

	/* Configure the System clock to have a frequency of 216 MHz */
	SystemClock_Config();

	// initializes LEDs
	BSP_LED_Init(LED1);

	// initializes SDRAM
	BSP_SDRAM_Init();

	// initializes LCD
	lcd_status = BSP_LCD_Init();
	while(lcd_status != LCD_OK);

	/* Initialize LTDC layer 0 iused for Hint */
	LCD_LayertInit(0, LAYER0_ADDRESS);
	BSP_LCD_SelectLayer(0);

	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);

	BSP_LCD_Clear(LCD_COLOR_WHITE);

	Display_DemoDescription();

//	HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
//	HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);

//	/* Update pitch : the draw is done on the whole physical X Size */
//	HAL_LTDC_SetPitch(&hltdc_discovery, BSP_LCD_GetXSize(), 0);
//
//	/* Display example brief   */
//	LCD_BriefDisplay();
//
//	/* Show first image */
//	CopyPicture((uint32_t *)Images[ImageIndex++], (uint32_t *)LAYER0_ADDRESS, 240, 160, 320, 240);
//
//	pending_buffer = 0;
//	active_area = LEFT_AREA;
//
//	HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
//
//	/* Send Display On DCS Command to display */
//	HAL_DSI_ShortWrite(&(hdsi_discovery),
//	                   0,
//	                   DSI_DCS_SHORT_PKT_WRITE_P1,
//	                   OTM8009A_CMD_DISPON,
//	                   0x00);

	// ----------- USB Device Initialization -----------
	/* Init Device Library */
	USBD_Init(&USBD_Device, &AUDIO_Desc, 0);

	/* Add Supported Class */
	USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);

	/* Add Interface callbacks for AUDIO Class */
	USBD_AUDIO_RegisterInterface(&USBD_Device, &audio_class_interface);

	/* Start Device Process */
	USBD_Start(&USBD_Device);

#if USE_AUDIO_TIMER_VOLUME_CTRL
	/* Configure timer for volume control handling */
	Timer_Init();
#endif /* USE_AUDIO_TIMER_VOLUME_CTRL */

	/* Infinite loop */
	while (1)
	{
		// if(pending_buffer < 0)
		// {
		// 	CopyPicture((uint32_t *)Images[ImageIndex++], (uint32_t *)LAYER0_ADDRESS, 240, 160, 320, 240);

		// 	if(ImageIndex >= 2)
		// 	{
		// 		ImageIndex = 0;
		// 	}
		// 	pending_buffer = 1;

		// 	HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
		// }
		// /* Wait some time before switching to next image */
		// HAL_Delay(2000);
	}
}

#if USE_AUDIO_TIMER_VOLUME_CTRL

/**
 * @brief  This function initialize timer to handle volume control.
 * @param  None
 * @retval Status
 */
static HAL_StatusTypeDef Timer_Init(void)
{
	/*##-1- Configure the TIM peripheral #######################################*/
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

	/* Compute the prescaler value to have TIM_VolumeChange counter clock equal to 10000 Hz */
	uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / 10000) - 1;

	/* Set TIM_VolumeChange instance */
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
		/* Initialization Error */
		Error_Handler();
	}

	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	/* Start Channel1 */
	if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
	{
		/* Starting Error */
		Error_Handler();
	}
	return HAL_OK;
}
#endif /* USE_AUDIO_TIMER_VOLUME_CTRL */

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

	/* Enable HSE Oscillator and activate PLL with HSE as source */
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

	/* Select PLLSAI output as USB clock source */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
	PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct)  != HAL_OK)
		Error_Handler();

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
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
	/* User may add here some code to deal with this error */
	while(1)
	{
	}
}
void USBD_error_handler(void)
{
	Error_Handler();
}

/**
 * @brief  On Error Handler on condition TRUE.
 * @param  condition : Can be TRUE or FALSE
 * @retval None
 */
static void OnError_Handler(uint32_t condition)
{
	if(condition)
	{
		BSP_LED_On(LED1);
		while(1)
		{
			;
		}  /* Blocking on error */
	}
}

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void)
{
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

/**
 * @brief  Initializes the DSI LCD.
 * The ititialization is done as below:
 *     - DSI PLL ititialization
 *     - DSI ititialization
 *     - LTDC ititialization
 *     - OTM8009A LCD Display IC Driver ititialization
 * @param  None
 * @retval LCD state
 */
static uint8_t LCD_Init(void)
{
	DSI_PHY_TimerTypeDef PhyTimings;

	/* Toggle Hardware Reset of the DSI LCD using
	 * its XRES signal (active low) */
	BSP_LCD_Reset();

	/* Call first MSP Initialize only in case of first initialization
	 * This will set IP blocks LTDC, DSI and DMA2D
	 * - out of reset
	 * - clocked
	 * - NVIC IRQ related to IP blocks enabled
	 */
	BSP_LCD_MspInit();

	/* LCD clock configuration */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 417 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 417 MHz / 5 = 83.4 MHz */
	/* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 83.4 / 2 = 41.7 MHz */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 417;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	/* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
	hdsi_discovery.Instance = DSI;

	HAL_DSI_DeInit(&(hdsi_discovery));

	dsiPllInit.PLLNDIV  = 100;
	dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
	dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;

	hdsi_discovery.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
	hdsi_discovery.Init.TXEscapeCkdiv = 0x4;
	HAL_DSI_Init(&(hdsi_discovery), &(dsiPllInit));

	/* Configure the DSI for Command mode */
	CmdCfg.VirtualChannelID      = 0;
	CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
	CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
	CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
	CmdCfg.ColorCoding           = DSI_RGB888;
	CmdCfg.CommandSize           = HACT;
	CmdCfg.TearingEffectSource   = DSI_TE_DSILINK;
	CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
	CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
	CmdCfg.AutomaticRefresh      = DSI_AR_ENABLE;
	CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
	HAL_DSI_ConfigAdaptedCommandMode(&hdsi_discovery, &CmdCfg);

	LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
	LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
	LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
	LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
	LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
	LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
	LPCmd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
	LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
	LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
	LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
	LPCmd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
	HAL_DSI_ConfigCommand(&hdsi_discovery, &LPCmd);

	/* Initialize LTDC */
	LTDC_Init();

	/* Start DSI */
	HAL_DSI_Start(&(hdsi_discovery));

	/* Configure DSI PHY HS2LP and LP2HS timings */
	PhyTimings.ClockLaneHS2LPTime = 35;
	PhyTimings.ClockLaneLP2HSTime = 35;
	PhyTimings.DataLaneHS2LPTime = 35;
	PhyTimings.DataLaneLP2HSTime = 35;
	PhyTimings.DataLaneMaxReadTime = 0;
	PhyTimings.StopWaitTime = 10;
	HAL_DSI_ConfigPhyTimer(&hdsi_discovery, &PhyTimings);

	/* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)*/
	OTM8009A_Init(OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);

	/* Reconfigure the DSI for HS Command mode */
	LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
	LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
	LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
	LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
	LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
	LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
	LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
	LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
	LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
	LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
	LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
	HAL_DSI_ConfigCommand(&hdsi_discovery, &LPCmd);

	// TODO: USB Audio breaks due to this line
	HAL_DSI_ConfigFlowControl(&hdsi_discovery, DSI_FLOW_CONTROL_BTA);

	/* Send Display Off DCS Command to display */
	HAL_DSI_ShortWrite(&(hdsi_discovery),
	                   0,
	                   DSI_DCS_SHORT_PKT_WRITE_P1,
	                   OTM8009A_CMD_DISPOFF,
	                   0x00);

	/* Refresh the display */
	HAL_DSI_Refresh(&hdsi_discovery);

	return LCD_OK;
}

/**
 * @brief  Initialize the LTDC
 * @param  None
 * @retval None
 */
void LTDC_Init(void)
{
	/* DeInit */
	HAL_LTDC_DeInit(&hltdc_discovery);

	/* LTDC Config */
	/* Timing and polarity */
	hltdc_discovery.Init.HorizontalSync = HSYNC;
	hltdc_discovery.Init.VerticalSync = VSYNC;
	hltdc_discovery.Init.AccumulatedHBP = HSYNC + HBP;
	hltdc_discovery.Init.AccumulatedVBP = VSYNC + VBP;
	hltdc_discovery.Init.AccumulatedActiveH = VSYNC + VBP + VACT;
	hltdc_discovery.Init.AccumulatedActiveW = HSYNC + HBP + HACT;
	hltdc_discovery.Init.TotalHeigh = VSYNC + VBP + VACT + VFP;
	hltdc_discovery.Init.TotalWidth = HSYNC + HBP + HACT + HFP;

	/* background value */
	hltdc_discovery.Init.Backcolor.Blue = 0;
	hltdc_discovery.Init.Backcolor.Green = 0;
	hltdc_discovery.Init.Backcolor.Red = 0;

	/* Polarity */
	hltdc_discovery.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	hltdc_discovery.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	hltdc_discovery.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	hltdc_discovery.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
	hltdc_discovery.Instance = LTDC;

	HAL_LTDC_Init(&hltdc_discovery);
}

/**
 * @brief  Display Example description.
 * @param  None
 * @retval None
 */
static void LCD_BriefDisplay(void)
{
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_FillRect(0, 0, 800, 112);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(0, 112, 800, 368);
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
	BSP_LCD_DisplayStringAtLine(1, (uint8_t *)"        LCD_DSI_CmdMode_PartialRefresh");
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"This example shows how to display images on LCD DSI using the partial");
	BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"Refresh method by splitting the display area.");
}

/**
 * @brief  Initializes the LCD layers.
 * @param  LayerIndex: Layer foreground or background
 * @param  FB_Address: Layer frame buffer
 * @retval None
 */
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
	LCD_LayerCfgTypeDef Layercfg;

	/* Layer Init */
	Layercfg.WindowX0 = 0;
	Layercfg.WindowX1 = BSP_LCD_GetXSize() / 2;
	Layercfg.WindowY0 = 0;
	Layercfg.WindowY1 = BSP_LCD_GetYSize();
	Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
	Layercfg.FBStartAdress = Address;
	Layercfg.Alpha = 255;
	Layercfg.Alpha0 = 0;
	Layercfg.Backcolor.Blue = 0;
	Layercfg.Backcolor.Green = 0;
	Layercfg.Backcolor.Red = 0;
	Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	Layercfg.ImageWidth = BSP_LCD_GetXSize() / 2;
	Layercfg.ImageHeight = BSP_LCD_GetYSize();

	HAL_LTDC_ConfigLayer(&hltdc_discovery, &Layercfg, LayerIndex);
}

/**
 * @brief  Converts a line to an ARGB8888 pixel format.
 * @param  pSrc: Pointer to source buffer
 * @param  pDst: Output color
 * @param  xSize: Buffer width
 * @param  ColorMode: Input color mode
 * @retval None
 */
static void CopyPicture(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{

	uint32_t destination = (uint32_t)pDst + (y * 800 + x) * 4;
	uint32_t source      = (uint32_t)pSrc;

	/*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
	hdma2d.Init.Mode         = DMA2D_M2M;
	hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
	hdma2d.Init.OutputOffset = 800 - xsize;
	hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA; /* No Output Alpha Inversion*/
	hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;/* No Output Red & Blue swap */

	/*##-2- DMA2D Callbacks Configuration ######################################*/
	hdma2d.XferCpltCallback  = NULL;

	/*##-3- Foreground Configuration ###########################################*/
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha = 0xFF;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
	hdma2d.LayerCfg[1].InputOffset = 0;
	hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
	hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

	hdma2d.Instance          = DMA2D;

	/* DMA2D Initialization */
	if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
		{
			if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK)
			{
				/* Polling For DMA transfer */
				HAL_DMA2D_PollForTransfer(&hdma2d, 100);
			}
		}
	}
}

static void Display_DemoDescription(void)
{
  char desc[50];

  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(0);

  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

  /* Clear the LCD */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"HOP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Versao A1", CENTER_MODE);

  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 65, (uint8_t *)utfprlogo);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t *)"Copyright (c) STMicroelectronics 2016", CENTER_MODE);

  BSP_LCD_SetFont(&Font24);
  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 90);
  BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 30, (uint8_t *)"Funcionalidades ativas:", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 60, (uint8_t *)"Audio USB | LCD Inicial", CENTER_MODE);
//  sprintf(desc,"%s example", "");
//  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)desc, CENTER_MODE);
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

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
