/* 
 **************************************************************************************************
 *
 * @file    : main.c
 * @author  : Bayrem GHARSELLAOUI
 * @date    : October 2021
 * @brief   : STM32 bluepill application
 * 
 **************************************************************************************************
 */

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "led.h"
#include "button.h"
#include "printf.h"

/*-----------------------------------------------------------------------------------------------*/
/* Private function prototypes                                                                   */
/*-----------------------------------------------------------------------------------------------*/
static void system_clock_config(void);
static void on_button_pressed(void);

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/
#define LED_BLINK_PERIOD_MS                      500

/*-----------------------------------------------------------------------------------------------*/
/* Private variables                                                                             */
/*-----------------------------------------------------------------------------------------------*/
static uint32_t u32LedToggleIndex;
static uint32_t u32ButtonPressIndex;

/*-----------------------------------------------------------------------------------------------*/
/* Exported functions                                                                            */
/*-----------------------------------------------------------------------------------------------*/
/** ***********************************************************************************************
  * @brief      Application entry point
  * @return     Nothing
  ********************************************************************************************** */
int main(void)
{
  u32LedToggleIndex = 0;
  u32ButtonPressIndex = 0;
  HAL_Init();
  system_clock_config();
  led_init();
  printf_init();
  button_init();
  button_register_callback(on_button_pressed);
  while(1)
  {
    printf("Toggling LED (%lu)\r\n", u32LedToggleIndex++);
    led_toggle();
    HAL_Delay(LED_BLINK_PERIOD_MS);
  }
}

/*-----------------------------------------------------------------------------------------------*/
/* Private functions                                                                             */
/*-----------------------------------------------------------------------------------------------*/
/** ***********************************************************************************************
  * @brief      Configure system clock at 216 MHz
  * @return     Nothing
  ********************************************************************************************** */
static void on_button_pressed(void)
{
  printf("Button is pressed (%lu)\r\n", u32ButtonPressIndex++);
}

/** ***********************************************************************************************
  * @brief      Configure system clock at 72 MHz
  * @return     Nothing
  ********************************************************************************************** */
static void system_clock_config(void)
{
  RCC_OscInitTypeDef stRccOscInit = {0};
  RCC_ClkInitTypeDef stRccClkInit = {0};

  /* Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  stRccOscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  stRccOscInit.HSEState = RCC_HSE_ON;
  stRccOscInit.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  stRccOscInit.HSIState = RCC_HSI_ON;
  stRccOscInit.PLL.PLLState = RCC_PLL_ON;
  stRccOscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  stRccOscInit.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&stRccOscInit);
  /** Initializes the CPU, AHB and APB buses clocks */
  stRccClkInit.ClockType = RCC_CLOCKTYPE_HCLK   | \
                           RCC_CLOCKTYPE_SYSCLK | \
                           RCC_CLOCKTYPE_PCLK1  | \
                           RCC_CLOCKTYPE_PCLK2;
  stRccClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  stRccClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
  stRccClkInit.APB1CLKDivider = RCC_HCLK_DIV2;
  stRccClkInit.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&stRccClkInit, FLASH_LATENCY_2);
}

/** ***********************************************************************************************
  * @brief      Systick timer interrupt handler
  * @return     Nothing
  ********************************************************************************************** */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/** ***********************************************************************************************
  * @brief      System hardfault interrupt handler
  * @return     Nothing
  ********************************************************************************************** */
void HardFault_Handler(void)
{
  while(1)
  {
  }
}