/*
 * eeprom.h
 *
 *  Created on: 25 дек. 2014 г.
 *      Author: Nik
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#define FLASH_EEPROM_MAGIC 0xCAFE0620
#define FLASH_EEPROM_EMPTY 0xFFFFFFFF
#define FLASH_PAGE_SIZE 0x400

void eepromInit(void);
int  eepromSaveParam(int type, void * src,int slen);
int  eepromLoadParam(int type, void * dst);
int  eepromVoidParam(int type);
void eepromErasePage(uint32_t address);
uint32_t eepromGetFreeAddress(uint32_t len);
void eepromCopyPageData(void);

extern uint32_t eeprom;

#endif /* EEPROM_H_ */
