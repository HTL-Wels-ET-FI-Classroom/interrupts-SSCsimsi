/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * Description of project
 * By Simon Schmidtgrabner||SSCsimsi
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
#include "ts_calibration.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define Colour_Amount 5
/* Private variables ---------------------------------------------------------*/
static volatile int timer_switch=0;
static volatile int timer_colour_switch=0;
static volatile int cnt[2]={0,0};
/* Private function prototypes -----------------------------------------------*/

//static int GetUserButtonPressed(void);
//static int GetTouchState (int *xCoord, int *yCoord);

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
	HAL_IncTick();
	cnt[timer_switch]++;
}
void EXTI0_IRQHandler(void){

	timer_switch=!timer_switch;

	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

}
void EXTI3_IRQHandler(void){

	timer_colour_switch++;

	if(timer_colour_switch==Colour_Amount){
		timer_colour_switch=0;
	}


	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);

}


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* MCU Configuration--------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize LCD and touch screen */
	LCD_Init();
	TS_Init(LCD_GetXSize(), LCD_GetYSize());
	/* touch screen calibration */
	//	TS_Calibration();

	/* Clear the LCD and display basic starter text */
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_SetBackColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font20);
		// printf Alternative

	LCD_DisplayStringAtLineMode(1, "EXTI Interrupt", CENTER_MODE);


	LCD_SetFont(&Font8);
	LCD_SetColors(LCD_COLOR_MAGENTA, LCD_COLOR_BLACK); // TextColor, BackColor
	LCD_DisplayStringAtLineMode(39, "copyright SSCsimsi||Simon Schmidtgrabner", CENTER_MODE);



	GPIO_InitTypeDef pin_PA0_SwitchTimer;
	pin_PA0_SwitchTimer.Alternate = 0;
	pin_PA0_SwitchTimer.Mode = GPIO_MODE_IT_RISING;
	pin_PA0_SwitchTimer.Pin = GPIO_PIN_0;
	pin_PA0_SwitchTimer.Pull = GPIO_NOPULL;
	pin_PA0_SwitchTimer.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOA, &pin_PA0_SwitchTimer);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);


	GPIO_InitTypeDef pin_PG3_SwitchTimerColour;
	pin_PG3_SwitchTimerColour.Alternate = 0;
	pin_PG3_SwitchTimerColour.Mode = GPIO_MODE_IT_RISING;
	pin_PG3_SwitchTimerColour.Pin = GPIO_PIN_3;
	pin_PG3_SwitchTimerColour.Pull = GPIO_PULLUP;
	pin_PG3_SwitchTimerColour.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOG, &pin_PG3_SwitchTimerColour);

	HAL_NVIC_EnableIRQ(EXTI3_IRQn);



	int colour_timers[Colour_Amount]={LCD_COLOR_GREEN,LCD_COLOR_BLUE,LCD_COLOR_RED,LCD_COLOR_BLACK,LCD_COLOR_WHITE};

	/* Infinite loop */
	while (1)
	{

		LCD_SetFont(&Font20);
		LCD_SetTextColor(colour_timers[timer_colour_switch]);
		LCD_SetPrintPosition(5, 0);
		printf("Timer1: %.2f", cnt[0]/1000.0);

		LCD_SetFont(&Font20);
		LCD_SetTextColor(colour_timers[timer_colour_switch]);
		LCD_SetPrintPosition(6, 0);
		printf("Timer2: %.2f", cnt[1]/1000.0);

	}
}

/**
 * Check if User Button has been pressed
 * @param none
 * @return 1 if user button input (PA0) is high
 */
/*
static int GetUserButtonPressed(void) {
	return (GPIOA->IDR & 0x0001);
}
*/
/**
 * Check if touch interface has been used
 * @param xCoord x coordinate of touch event in pixels
 * @param yCoord y coordinate of touch event in pixels
 * @return 1 if touch event has been detected
 */
/*
static int GetTouchState (int* xCoord, int* yCoord) {
	void    BSP_TS_GetState(TS_StateTypeDef *TsState);
	TS_StateTypeDef TsState;
	int touchclick = 0;

	TS_GetState(&TsState);
	if (TsState.TouchDetected) {
		*xCoord = TsState.X;
		*yCoord = TsState.Y;
		touchclick = 1;
		if (TS_IsCalibrationDone()) {
			*xCoord = TS_Calibration_GetX(*xCoord);
			*yCoord = TS_Calibration_GetY(*yCoord);
		}
	}

	return touchclick;
}

*/

