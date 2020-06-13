#ifndef FLASH_H_
#define FLASH_H_

#define FLASH_KEY1			   ((uint32_t)0x45670123)
#define FLASH_KEY2			   ((uint32_t)0xCDEF89AB)
#define FLASH_PAGE				 ((uint8_t)0x7F)

uint8_t flash_ready(void) ;
void flash_erase_page(uint32_t address) ;
void flash_unlock(void) ;
void flash_lock() ;
void flash_write_word(uint32_t address,uint32_t data) ;
void flash_read(uint32_t address,void * dest,int lenBytes);
void flash_write(uint32_t flashAddr, void * data,int lenBytes);

#endif /* FLASH_H_ */
