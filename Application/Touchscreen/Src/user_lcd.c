#include "user_lcd.h"

// pictures -----------------------------------------------------------
#include "utfprlogo.h"

// private defines -------------------------------------------------------------
#define VSYNC             1
#define VBP               1
#define VFP               1
#define VACT              480
#define HSYNC             1
#define HBP               1
#define HFP               1
#define HACT              400 // screen divided into two areas

#define INVALID_AREA      0
#define LEFT_AREA         1
#define RIGHT_AREA        2
#define WVGA_RES_X        800
#define WVGA_RES_Y        480

#define ARGB8888_BYTE_PER_PIXEL 4
#define RGB565_BYTE_PER_PIXEL   2
#define LCD_FRAME_BUFFER        0xC0000000
#define LAYER_SIZE_X            240
#define LAYER_SIZE_Y            130
#define LAYER_BYTE_PER_PIXEL    RGB565_BYTE_PER_PIXEL

// private function declarations --------------------------------------
static void     LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
static void     Display_StartupScreen(void);
static uint8_t  CopyImageToLcdFrameBuffer(void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize, uint16_t x, uint16_t y);
void LCD_DisplayPlusButton(uint8_t buttonIndex);
void LCD_DisplayKnob(uint8_t knobIndex);

// variables ----------------------------------------------------------
// !! THE NUMBER_OF_CIRCLE_BUTTONS DEFINE HAS TO BE UP TO DATE WITH THE NUMBER OF BUTTONS DEFINED HERE
CircleButtonTypeDef circleButtons[] = {
  //  x,   y,  r,               color,       text color,        on,     off,  pressed, independent, active
  { 100, 420, 50, LCD_COLOR_BURGUNDY,   LCD_COLOR_BLACK, "Off",     "",       true,     false,      false},
  { 250, 420, 50, LCD_COLOR_MINT_GREEN, LCD_COLOR_BLACK, "Volume",  "",       false,    false,      false},
  { 400, 420, 50, LCD_COLOR_MINT_GREEN, LCD_COLOR_BLACK, "Low Pass",     "",       false,    false,      false},
  { 550, 420, 50, LCD_COLOR_MINT_GREEN, LCD_COLOR_BLACK, "Biquad",     "",       false,    false,      false},
  { 700, 420, 50, LCD_COLOR_MINT_GREEN, LCD_COLOR_BLACK, "NYI",     "",       false,    false,      false},
  { 650, 120, 50, LCD_COLOR_LIGHTBLUE,  LCD_COLOR_BLACK, "Sending", "Debug",  false,    true,       false}
};

IncrementButton plusButtons[] = {
  { 100, 150, 50, 50, LCD_COLOR_BLACK, LCD_COLOR_WHITE, "+", false, 1},
  // { 150, 100, 30, 30, LCD_COLOR_BLACK, LCD_COLOR_WHITE, "+", false, 1}
};

SliderKnob sliderKnobs[] = {
  { 50, 20, 100, 300, LCD_COLOR_BLUE, 160, 20, false, 5, 5},
  { 150, 20, 100, 300, LCD_COLOR_BLUE, 160, 20, false, 5, 5},
  { 250, 20, 100, 300, LCD_COLOR_BLUE, 160, 20, false, 5, 5},
  { 350, 20, 100, 300, LCD_COLOR_BLUE, 160, 20, false, 5, 5},
  { 450, 20, 100, 300, LCD_COLOR_BLUE, 160, 20, false, 5, 5},
};

// external variable declarations -------------------------------------
extern LTDC_HandleTypeDef hltdc_discovery;
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
void LCD_Init(void)
{
  uint8_t lcd_status = LCD_OK;

  lcd_status = BSP_LCD_Init();
  while(lcd_status != LCD_OK);

  // Initialize LTDC layer 0 iused for Hint
  LCD_LayertInit(0, LAYER0_ADDRESS);
  BSP_LCD_SelectLayer(0);

  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);

  BSP_LCD_Clear(LCD_COLOR_WHITE);

  Display_StartupScreen();
}

/**
 * @brief  Initializes the LCD layers.
 * @param  LayerIndex: Layer foreground or background
 * @param  FB_Address: Layer frame buffer
 * @retval None
 */
static void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
  LCD_LayerCfgTypeDef Layercfg;

  // Layer Init
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

static void Display_StartupScreen(void)
{

  // sets lcd foreground layer
  BSP_LCD_SelectLayer(0);

  // clears the lcd
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  // sets the lcd text color and font
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);

  // displays header messages
  // BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"HOP", CENTER_MODE);
  // BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Versao W26", CENTER_MODE);

  // displays footer
  // BSP_LCD_SetFont(&Font12);
  // BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 20, (uint8_t *)"Apple Pie - commit b1bb6358", CENTER_MODE);

  // draws logo picture
  // BSP_LCD_DrawPicture(utfprlogo, UTFPR_LOGO_WIDTH, UTFPR_LOGO_HEIGHT, (WVGA_RES_X / 2) - (UTFPR_LOGO_WIDTH / 2), 20);
  BSP_LCD_DrawPicture(utfprlogo, UTFPR_LOGO_WIDTH, UTFPR_LOGO_HEIGHT, 20, 20);

  // displays content messages
  #define Y_BAR_POSITION -60
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_SetTextColor(LCD_COLOR_UTFPRYELLOW);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize() / 2 + Y_BAR_POSITION, BSP_LCD_GetXSize(), 90);

  BSP_LCD_SetBackColor(LCD_COLOR_UTFPRYELLOW);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + Y_BAR_POSITION + 15, (uint8_t *)"Versao Horoscope", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + Y_BAR_POSITION + 45, (uint8_t *)"Branch biquad", CENTER_MODE);

  for(uint8_t i = 0; i < NUMBER_OF_CIRCLE_BUTTONS; i++)
  {
    LCD_UpdateButton(i, circleButtons[i].isPressed, false);
  }

  // LCD_DisplayPlusButton(0);

  for(int i = 0; i < NUMBER_OF_SLIDER_BUTTONS; i++)
  {
    LCD_DisplayKnob(i);
  }
}

/**
 * @brief  Copy and convert image (LAYER_SIZE_X, LAYER_SIZE_Y) of format RGB565
 * to LCD frame buffer area centered in WVGA resolution.
 * The area of copy is of size (LAYER_SIZE_X, LAYER_SIZE_Y) in ARGB8888.
 * @param  pSrc: Pointer to source buffer : source image buffer start here
 * @param  pDst: Pointer to destination buffer LCD frame buffer center area start here
 * @param  xSize: Buffer width (LAYER_SIZE_X here)
 * @param  ySize: Buffer height (LAYER_SIZE_Y here)
 * @retval LCD Status : LCD_OK or LCD_ERROR
 */
static uint8_t CopyImageToLcdFrameBuffer(void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize, uint16_t x, uint16_t y)
{

  uint32_t destination = (uint32_t)pDst + (y * 800 + x) * 4;
  DMA2D_HandleTypeDef hdma2d_discovery;
  HAL_StatusTypeDef hal_status = HAL_OK;
  uint8_t lcd_status = LCD_ERROR;

  // Configure the DMA2D Mode, Color Mode and output offset
  hdma2d_discovery.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_discovery.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;// Output color out of PFC
  hdma2d_discovery.Init.AlphaInverted = DMA2D_REGULAR_ALPHA; // No Output Alpha Inversion
  hdma2d_discovery.Init.RedBlueSwap   = DMA2D_RB_REGULAR;// No Output Red & Blue swap

  // Output offset in pixels == nb of pixels to be added at end of line to come to the
  // first pixel of the next line : on the output side of the DMA2D computation
  // TODO: GENERALIZE
  hdma2d_discovery.Init.OutputOffset = (WVGA_RES_X - UTFPR_LOGO_WIDTH);

  // Foreground Configuration
  hdma2d_discovery.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_discovery.LayerCfg[1].InputAlpha = 0xFF; // fully opaque
  hdma2d_discovery.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d_discovery.LayerCfg[1].InputOffset = 0;
  hdma2d_discovery.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; // No ForeGround Red/Blue swap
  hdma2d_discovery.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; // No ForeGround Alpha inversion

  hdma2d_discovery.Instance = DMA2D;

  // DMA2D Initialization
  if(HAL_DMA2D_Init(&hdma2d_discovery) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_discovery, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d_discovery, (uint32_t)pSrc, destination, xSize, ySize) == HAL_OK)
      {
        // Polling For DMA transfer
        hal_status = HAL_DMA2D_PollForTransfer(&hdma2d_discovery, 10);
        if(hal_status == HAL_OK)
        {
          // return good status on exit
          lcd_status = LCD_OK;
        }
      }
    }
  }

  return(lcd_status);
}


void LCD_DisplayPlusButton(uint8_t buttonIndex)
{
  IncrementButton* button = &plusButtons[buttonIndex];

  BSP_LCD_SetTextColor(button->color);
  BSP_LCD_FillRect(button->x, button->y, button->width, button->height);


  BSP_LCD_SetBackColor(button->color);
  BSP_LCD_SetTextColor(button->textColor);
  BSP_LCD_DisplayStringAt(button->x + button->width + 5, button->y + button->height - 20, (uint8_t *)button->text, LEFT_MODE);
}

void LCD_DisplayKnob(uint8_t knobIndex)
{
  SliderKnob* knob = &sliderKnobs[knobIndex];

  BSP_LCD_SetTextColor(knob->sliderColor);
  BSP_LCD_FillRect(knob->sliderX, knob->sliderY, knob->sliderWidth, knob->sliderHeight);

  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(knob->sliderX + knob->sliderWidth / 2, knob->knobY, knob->knobRadius);
}


void LCD_UpdateButton(uint8_t buttonIndex, bool isPressed, bool shouldToggleOtherButtons)
{
  CircleButtonTypeDef* button = &circleButtons[buttonIndex];

  if(button->isActive && button->isPressed == isPressed)
  {
    return;
  }

  button->isPressed = isPressed;

  if(!button->isActive)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillCircle(button->x, button->y, button->radius);
    button->isActive = true;
  }

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillCircle(button->x, button->y, button->radius);

  BSP_LCD_SetTextColor(button->isPressed ? button->color : LCD_COLOR_LIGHTGRAY);
  BSP_LCD_FillCircle(button->x, button->y, button->radius - 5);

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(button->x - button->radius - 12, button->y - button->radius - 24, (uint8_t *)(button->isPressed ? button->offText : button->onText), LEFT_MODE);

  BSP_LCD_SetTextColor(button->textColor);
  BSP_LCD_DisplayStringAt(button->x - button->radius - 12, button->y - button->radius - 24, (uint8_t *)(button->isPressed ? button->onText : button->offText), LEFT_MODE);

  if(!button->isIndependent && shouldToggleOtherButtons)
  {

    for(int i = 0; i < NUMBER_OF_CIRCLE_BUTTONS; i++)
    {
      if(i != buttonIndex && !circleButtons[i].isIndependent)
        LCD_UpdateButton(i, false, false);
    }
  }
}

void LCD_UpdateWatchdog (uint32_t* watchdogCounter)
{
  char text[5];
  sprintf(text, "%04u", ((unsigned int)*watchdogCounter));
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)text, RIGHT_MODE);
  *watchdogCounter = *watchdogCounter + 1;
  if(*watchdogCounter > 9999)
    *watchdogCounter = 0;
}


void BSP_LCD_DrawPicture(const uint8_t* image, uint32_t width, uint32_t height, uint32_t xPosition, uint32_t yPosition )
{
  CopyImageToLcdFrameBuffer((void*)image, (void*)(LCD_FRAME_BUFFER), width, height, xPosition, yPosition);
}
