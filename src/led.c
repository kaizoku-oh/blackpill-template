#include "stm32f1xx_hal.h"

#define LED_PIN                                  GPIO_PIN_12
#define LED_GPIO_PORT                            GPIOB
#define LED_GPIO_CLK_ENABLE()                    __HAL_RCC_GPIOB_CLK_ENABLE()

void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  LED_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

void LED_On(void)
{
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET);
}

void LED_Off(void)
{
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);
}

void LED_Toggle(void)
{
  HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
}
