#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "led.h"
#include "button.h"
#include "printf.h"

static void SystemClock_Config(void);
static void On_Button_Presssed(void);
static void Error_Handler(void);

static uint32_t ledToggleIndex;
static uint32_t buttonPressIndex;

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  LED_Init();
  PRINTF_Init();
  BUTTON_Init();
  BUTTON_Register_Callback(On_Button_Presssed);
  ledToggleIndex = 0;
  buttonPressIndex = 0;
  while(1)
  {
    printf("Toggling LED (%lu)\r\n", ledToggleIndex++);
    LED_Toggle();
    HAL_Delay(1000);
  }
}

static void On_Button_Presssed(void)
{
  printf("Button is pressed (%lu)\r\n", buttonPressIndex++);
}

static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK   | \
                                RCC_CLOCKTYPE_SYSCLK | \
                                RCC_CLOCKTYPE_PCLK1  | \
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void HardFault_Handler(void)
{
  while(1)
  {
  }
}