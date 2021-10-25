#include <unistd.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

#define UART_RX_BUFFER_SIZE                      32

#define PRINTF_USART_INSTANCE                    USART1
#define PRINTF_USART_BAUDRATE                    115200
#define PRINTF_USART_PORT                        GPIOA
#define PRINTF_USART_TX_PIN                      GPIO_PIN_9
#define PRINTF_USART_RX_PIN                      GPIO_PIN_10
#define PRINTF_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOA_CLK_ENABLE()
#define PRINTF_USART_CLK_ENABLE()                __HAL_RCC_USART1_CLK_ENABLE()

static UART_HandleTypeDef usartHandle;
static uint32_t uartRxIndex;
static uint8_t uartRxByte;
static uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];

void PRINTF_Init(void)
{
  usartHandle.Instance = PRINTF_USART_INSTANCE;
  usartHandle.Init.BaudRate = PRINTF_USART_BAUDRATE;
  usartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  usartHandle.Init.StopBits = UART_STOPBITS_1;
  usartHandle.Init.Parity = UART_PARITY_NONE;
  usartHandle.Init.Mode = UART_MODE_TX_RX;
  usartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  usartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&usartHandle);
}

void HAL_UART_MspInit(UART_HandleTypeDef* handle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(PRINTF_USART_INSTANCE == handle->Instance)
  {
    PRINTF_USART_CLK_ENABLE();
    PRINTF_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = PRINTF_USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PRINTF_USART_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = PRINTF_USART_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PRINTF_USART_PORT, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

int _write(int file, char *ptr, int len)
{
  int retVal;

  if((STDOUT_FILENO == file) || (STDERR_FILENO == file))
  {
    retVal = (HAL_OK == HAL_UART_Transmit(&usartHandle,
                                          (uint8_t *)ptr,
                                          (len * sizeof(uint8_t)),
                                          HAL_MAX_DELAY))
             ?(len * sizeof(uint8_t))
             :-1;
  }
  else
  {
    retVal = -1;
  }
  return retVal;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
  if(PRINTF_USART_INSTANCE == handle->Instance)
  {
    /* Circular buffer effect: if the end of the reception buffer is reached, wrap the buffer */
    if(UART_RX_BUFFER_SIZE == uartRxIndex)
    {
      uartRxIndex = 0;
    }
    /* Store received byte in buffer */
    uartRxBuffer[uartRxIndex++] = uartRxByte;
    /* On each received byte reactivate USART reception for 1 byte */
    HAL_UART_Receive_IT(handle, &uartRxByte, sizeof(uint8_t));
    /* Echo back received byte */
    HAL_UART_Transmit(&usartHandle, &uartRxByte, sizeof(uint8_t), HAL_MAX_DELAY);
  }
}

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&usartHandle);
}
