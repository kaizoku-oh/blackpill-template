#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "button.h"

#define BUTTON_PIN                               GPIO_PIN_13
#define BUTTON_GPIO_PORT                         GPIOC
#define BUTTON_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOC_CLK_ENABLE()

static button_callback_t buttonCallback;

void BUTTON_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  BUTTON_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStruct);
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void BUTTON_Register_Callback(button_callback_t callback)
{
  if(callback)
  {
    buttonCallback = callback;
  }
}

bool BUTTON_Is_Pressed(void)
{
  bool result;

  result = (GPIO_PIN_SET == HAL_GPIO_ReadPin(BUTTON_GPIO_PORT, BUTTON_PIN))?true:false;
  return result;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(BUTTON_PIN == GPIO_Pin)
  {
    if(buttonCallback)
    {
      buttonCallback();
    }
  }
}

void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
