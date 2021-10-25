#include "stm32f1xx_hal.h"

#include <unistd.h>
#include <stdio.h>

#define LED_PIN                                  GPIO_PIN_12
#define LED_GPIO_PORT                            GPIOB
#define LED_GPIO_CLK_ENABLE()                    __HAL_RCC_GPIOB_CLK_ENABLE()

#define BUTTON_PIN                               GPIO_PIN_13
#define BUTTON_GPIO_PORT                         GPIOC
#define BUTTON_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOC_CLK_ENABLE()

#define PRINTF_USART_PORT                        GPIOA
#define PRINTF_USART_TX_PIN                      GPIO_PIN_9
#define PRINTF_USART_RX_PIN                      GPIO_PIN_10
#define PRINTF_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOA_CLK_ENABLE()
#define PRINTF_USART_CLK_ENABLE()                __HAL_RCC_USART1_CLK_ENABLE()

#define UART_RX_BUFFER_SIZE                      32

static void LED_Init(void);
static void Button_Init(void);
static void UART_Init(void);
static void SystemClock_Config(void);

UART_HandleTypeDef huart1;
uint32_t ledToggleIndex;
uint32_t uartRxIndex;
uint8_t uartRxByte;
uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];

int main(void)
{
  ledToggleIndex = 0;
  uartRxIndex = 0;
  HAL_Init();
  SystemClock_Config();
  UART_Init();
  LED_Init();
  Button_Init();
  /* Activate USART reception for 1 byte */
  HAL_UART_Receive_IT(&huart1, &uartRxByte, sizeof(uint8_t));
  while(1)
  {
    printf("Toggling LED (%lu)\r\n", ledToggleIndex++);
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    HAL_Delay(1000);
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
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

static void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  LED_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

static void Button_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  BUTTON_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStruct);
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(BUTTON_PIN == GPIO_Pin)
  {
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(USART1 == huart->Instance)
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

static void UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(USART1 == huart->Instance)
  {
    /* Circular buffer effect: if the end of the reception buffer is reached, wrap the buffer */
    if(UART_RX_BUFFER_SIZE == uartRxIndex)
    {
      uartRxIndex = 0;
    }
    /* Store received byte in buffer */
    uartRxBuffer[uartRxIndex++] = uartRxByte;
    /* On each received byte reactivate USART reception for 1 byte */
    HAL_UART_Receive_IT(huart, &uartRxByte, sizeof(uint8_t));
    /* Echo back received byte */
    HAL_UART_Transmit(&huart1, &uartRxByte, sizeof(uint8_t), HAL_MAX_DELAY);
  }
}

int _write(int file, char *ptr, int len)
{
  int retVal;

  if((STDOUT_FILENO == file) || (STDERR_FILENO == file))
  {
    retVal = (HAL_OK == HAL_UART_Transmit(&huart1,
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

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}

void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
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
