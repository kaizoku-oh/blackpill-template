/* 
 **************************************************************************************************
 *
 * @file    : flash.h
 * @author  : Bayrem GHARSELLAOUI
 * @date    : October 2021
 * @brief   : STM32 bluepill internal flash module header file
 * 
 **************************************************************************************************
 */
/* Example:
  uint8_t tu08Data[8];

  flash_save((uint8_t *)"Hello", (sizeof("Hello")-1));
  flash_read(0x00000000, tu08Data, 5);
  printf("read data: %.*s", 5, tu08Data);
*/
#ifndef __FLASH_H__
#define __FLASH_H__

/*-----------------------------------------------------------------------------------------------*/
/* Functions                                                                                     */
/*-----------------------------------------------------------------------------------------------*/
bool flash_save(uint8_t *pu08Data, uint8_t u08Length);
bool flash_read(uint32_t u32Address, uint8_t *pu08Data, uint8_t u08Length);

#endif /* __FLASH_H__ */