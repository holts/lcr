#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "flash.h"

//----------------------------------------------------------------------------
uint8_t flash_ready(void) {	return !(FLASH->SR & FLASH_SR_BSY);}
//----------------------------------------------------------------------------
void flash_erase_page(uint32_t address) {
	FLASH->CR|= FLASH_CR_PER;
	FLASH->AR = address;
	FLASH->CR|= FLASH_CR_STRT;
	while(!flash_ready())		;
	FLASH->CR&= ~FLASH_CR_PER;
}
//----------------------------------------------------------------------------
void flash_unlock(void) {	  FLASH->KEYR = FLASH_KEY1;	  FLASH->KEYR = FLASH_KEY2;}
//----------------------------------------------------------------------------
void flash_lock() {	FLASH->CR |= FLASH_CR_LOCK;}
//----------------------------------------------------------------------------
void flash_write_word(uint32_t address,uint32_t data) {

	FLASH->CR |= FLASH_CR_PG;

	while(!flash_ready())		;

	*(__IO uint16_t*)address = (uint16_t)(data&0xFFFF);

	while(!flash_ready())		;

	FLASH->CR &= ~(FLASH_CR_PG);

}
//----------------------------------------------------------------------------
void flash_read(uint32_t address,void * dest,int lenBytes) {
	uint16_t * dst = (uint16_t * )dest;
	volatile uint16_t * src = (uint16_t * )address;
	for(int i =0;i<(lenBytes>>1);i++) dst[i] = src[i];
}
//----------------------------------------------------------------------------
void flash_write(uint32_t flashAddr, void * data,int lenBytes)
{
uint16_t * buf = (uint16_t * )data;

	flash_unlock();

	for(int i =0;i<lenBytes;i+=2)
	{
	flash_write_word(flashAddr + i,buf[i>>1]);
	}
	flash_lock();

}
//----------------------------------------------------------------------------
