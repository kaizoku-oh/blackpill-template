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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f1xx_hal.h"

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/
#define FLASH_LAST_PAGE_START_ADDRESS             (0x0800FC00)
#define FLASH_LAST_PAGE_SIZE                      (1024)
#define FLASH_DOUBLE_WORD_SIZE                    (sizeof(uint64_t))
#define FLASH_WORD_SIZE                           (sizeof(uint32_t))
#define FLASH_HALF_WORD_SIZE                      (sizeof(uint16_t))

/*-----------------------------------------------------------------------------------------------*/
/* Exported functions                                                                            */
/*-----------------------------------------------------------------------------------------------*/
/** ***********************************************************************************************
  * @brief      Save data to internal flash
  * @param      pu08Data pointer to input data
  * @param      u08Length data length
  * @return     Nothing
  ********************************************************************************************** */
bool flash_save(uint8_t *pu08Data, uint8_t u08Length)
{
  bool bResult;
  uint8_t u08Index;
  uint32_t u32Address;
  uint16_t u16TmpHalfWord;
  uint8_t u08PaddingCount;
  uint32_t u32ErrorStatus = 0;
  FLASH_EraseInitTypeDef stFlashEraseInit = {0};

  bResult = false;
  /* 0. Check parameters */
  if(pu08Data && u08Length && u08Length < FLASH_LAST_PAGE_SIZE)
  {
    /* 1. Unlock the flash */
    if(HAL_OK == HAL_FLASH_Unlock())
    {
      /* 2. Erase the last page of the flash */
      stFlashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
      stFlashEraseInit.PageAddress = FLASH_LAST_PAGE_START_ADDRESS;
      stFlashEraseInit.NbPages = 1;
      if(HAL_OK == HAL_FLASHEx_Erase(&stFlashEraseInit, &u32ErrorStatus))
      {
        /* 3. Calculate padding count,
         * since the smallest data unit tha can be written to flash is 2 bytes (half word):
         * the padding count will be a value of 0 or 1
         */
        u08PaddingCount = u08Length % FLASH_HALF_WORD_SIZE;
        /* 4. Loop through the user data 2 bytes (half word) at a time */
        u32Address = FLASH_LAST_PAGE_START_ADDRESS;
        for(u08Index=0; u08Index<u08Length; u08Index+=FLASH_HALF_WORD_SIZE)
        {
          /* Copy 2 bytes of user data to the temporary half word variable and write it to flash */
          memcpy(&u16TmpHalfWord, pu08Data+u08Index, FLASH_HALF_WORD_SIZE);
          if(HAL_OK == HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, u32Address, u16TmpHalfWord))
          {
            bResult = true;
            u32Address += FLASH_HALF_WORD_SIZE;
          }
          else
          {
            bResult = false;
            break;
          }
        }
        /* 5. Write padding to flash if it's needed */
        if(true == bResult)
        {
          if(u08PaddingCount)
          {
            u16TmpHalfWord = 0xFFFF;
            if(HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, u32Address, u16TmpHalfWord))
            {
              bResult = false;
            }
          }
        }
      }
    }
  }
  return bResult;
}

/** ***********************************************************************************************
  * @brief      Read data from internal flash
  * @param      u32Address address to read from
  * @param      pu08Data pointer to output data
  * @param      u08Length data length
  * @return     Nothing
  ********************************************************************************************** */
bool flash_read(uint32_t u32Address, uint8_t *pu08Data, uint8_t u08Length)
{
  bool bResult;
  uint8_t *pu08ReadPointer;

  /* 1. Check parameters and that the given address is in the range of the last flash page */
  if(pu08Data &&
     u08Length &&
     (u08Length < FLASH_LAST_PAGE_SIZE) &&
     ((u32Address >= FLASH_LAST_PAGE_START_ADDRESS) &&
      (u32Address < (FLASH_LAST_PAGE_START_ADDRESS + FLASH_LAST_PAGE_SIZE))) &&
     ((u32Address + u08Length) < (FLASH_LAST_PAGE_START_ADDRESS + FLASH_LAST_PAGE_SIZE)))
  {
    pu08ReadPointer = (uint8_t *)FLASH_LAST_PAGE_START_ADDRESS;
    memcpy(pu08Data, pu08ReadPointer, u08Length);
    bResult = true;
  }
  else
  {
    bResult = false;
  }
  return bResult;
}
