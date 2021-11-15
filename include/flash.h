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

  static void _test_flash(void)
  {
    uint8_t tu08Data[12];

    if(flash_save((uint8_t *)"Hello world!", (sizeof("Hello world!")-1)))
    {
      printf("Data saved to flash successfully!\r\n");
      if(flash_read(0x0800FC00, tu08Data, 12))
      {
        printf("Data read from flash successfully!\r\n");
        printf("Data: %.*s\r\n", 12, tu08Data);
      }
      else
      {
        printf("Failed to read data from flash\r\n");
      }
    }
    else
    {
      printf("Failed to save data to flash\r\n");
    }
  }
*/
#ifndef __FLASH_H__
#define __FLASH_H__

/*-----------------------------------------------------------------------------------------------*/
/* Functions                                                                                     */
/*-----------------------------------------------------------------------------------------------*/
bool flash_save(uint8_t *pu08Data, uint8_t u08Length);
bool flash_read(uint32_t u32Address, uint8_t *pu08Data, uint8_t u08Length);

#endif /* __FLASH_H__ */