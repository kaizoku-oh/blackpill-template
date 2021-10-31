/* 
 **************************************************************************************************
 *
 * @file    : flash.c
 * @author  : Bayrem GHARSELLAOUI
 * @brief   : STM32 bluepill internal flash module for saving and reading data from a 1kb page
 * 
 **************************************************************************************************
 */

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#include <string.h>
#include "stm32f1xx_hal.h"

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/
#define FLASH_LAST_PAGE_START_ADDRESS             (0x0800FC00)
#define FLASH_DOUBLE_WORD_SIZE                    (sizeof(uint64_t))
#define FLASH_WORD_SIZE                           (sizeof(uint32_t))
#define FLASH_HALF_WORD_SIZE                      (sizeof(uint16_t))

/*-----------------------------------------------------------------------------------------------*/
/* Private variables                                                                             */
/*-----------------------------------------------------------------------------------------------*/
static uint8_t tu08Data[FLASH_DOUBLE_WORD_SIZE];

/*-----------------------------------------------------------------------------------------------*/
/* Exported functions                                                                            */
/*-----------------------------------------------------------------------------------------------*/
/** ***********************************************************************************************
  * @brief      Save data to internal flash
  * @param      pu08Data pointer to input data
  * @param      u08Length data length
  * @return     Nothing
  ********************************************************************************************** */
void flash_save(uint8_t *pu08Data, uint8_t u08Length)
{
  uint8_t u08Index;
  uint32_t u32ErrorStatus = 0;
  FLASH_EraseInitTypeDef stFlashEraseInit = {0};

  /* 1. Unlock the flash */
  HAL_FLASH_Unlock();
  /* 2. Erase the last page of the flash */
  stFlashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
  stFlashEraseInit.PageAddress = FLASH_LAST_PAGE_START_ADDRESS;
  stFlashEraseInit.NbPages = 1;
  HAL_FLASHEx_Erase(&stFlashEraseInit, &u32ErrorStatus);
  /* 3. Write */
  if(u08Length < FLASH_DOUBLE_WORD_SIZE)
  {
    if(u08Length < FLASH_WORD_SIZE)
    {
    }
    else
    {
    }
  }
  else
  {
  }
  
  for(u08Index=0; u08Index<u08Length; u08Index++)
  {
  }
  uint16_t FData = 0xCAFE;
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_LAST_PAGE_START_ADDRESS, FData);
}

/** ***********************************************************************************************
  * @brief      Read data from internal flash
  * @param      pu08Data pointer to output data
  * @param      u08Length data length
  * @return     Nothing
  ********************************************************************************************** */
void flash_read(uint32_t u32Address, uint8_t *pu08Data, uint8_t u08Length)
{
}
