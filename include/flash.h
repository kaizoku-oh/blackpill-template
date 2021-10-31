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

#ifndef __FLASH_H__
#define __FLASH_H__

/*-----------------------------------------------------------------------------------------------*/
/* Functions                                                                                     */
/*-----------------------------------------------------------------------------------------------*/
void flash_save(uint8_t *pu08Data, uint8_t u08Length);
void flash_read(uint32_t u32Address, uint8_t *pu08Data, uint8_t u08Length);

#endif /* __FLASH_H__ */