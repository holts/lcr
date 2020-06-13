
#include "stdint.h"

#include "flash.h"
#include "eeprom.h"

uint32_t eeprom;

const uint16_t __attribute__ (( aligned(1024) )) __attribute__ ((section (".eeprom"))) eep1[512]  = { [0 ... 511] = 0xffff };
const uint16_t __attribute__ (( aligned(1024) )) __attribute__ ((section (".eeprom"))) eep2[512]  = { [0 ... 511] = 0xffff };


typedef struct
{
	uint16_t type;
	uint16_t len;
} eeprom_record_hdr;

//----------------------------------------------------------------------------
void eepromErasePage(uint32_t address)
{
	flash_unlock();
	flash_erase_page((uint32_t)address);
	flash_lock();
}
//----------------------------------------------------------------------------
uint32_t eepromGetFreeAddress(uint32_t slen)
{
uint32_t addr = (eeprom + 4);
eeprom_record_hdr hdr;

do {
	flash_read(addr,&hdr,sizeof(hdr));
	if(hdr.len != 0xFFFF) addr += hdr.len;
}while( (hdr.len != 0xFFFF) && ( addr < (eeprom + FLASH_PAGE_SIZE) ) );

if( addr >= (eeprom + FLASH_PAGE_SIZE - slen)) return (0);

return (addr);
}
//----------------------------------------------------------------------------
int eepromSaveParam(int type, void * src,int slen) // 0 - ok
{
	char data[64];
	uint32_t addr = (eeprom + 4);
	eeprom_record_hdr hdr;
	uint32_t flag = 0;


	if( eepromLoadParam( type, data ) > 0 )
	{
	for(int i =0;i<slen;i++)
		if( ((char * )src)[i] != data[i] ) flag++;

	if(flag == 0) return (2);
	}

	eepromVoidParam(type);

	addr = eepromGetFreeAddress( slen + sizeof(hdr) );

	if(addr == 0 ) eepromCopyPageData();

	addr = eepromGetFreeAddress( slen + sizeof(hdr) );

	hdr.len = slen + sizeof(hdr);
	hdr.type = type;

	flash_write(addr,&hdr,sizeof(hdr));
	flash_write(addr + sizeof(hdr),src,slen);

	return (1);
}
//----------------------------------------------------------------------------
int eepromVoidParam(int type) // 0 - ok
{
	uint32_t addr = eeprom + 4;
	eeprom_record_hdr hdr;

	do {
		flash_read(addr, &hdr, sizeof(hdr) );

		if(hdr.type == type )	{
			hdr.type = 0;
			flash_write(addr, &hdr, sizeof(hdr) ); // zero type
		}
		if(hdr.len != 0xFFFF) addr += hdr.len;

	}while( (hdr.len != 0xFFFF) && ( addr < ((uint32_t)eeprom) + FLASH_PAGE_SIZE ) );

	return (0);
}
//----------------------------------------------------------------------------
int eepromLoadParam(int type, void * dst)
{
	int n = 0;
	uint32_t addr = eeprom + 4;
	eeprom_record_hdr hdr;

	do {
		flash_read( addr, &hdr, sizeof(hdr) );

		if( hdr.type == type ){ flash_read( addr +  sizeof(hdr), dst, hdr.len - sizeof(hdr) );	n++;	}

		addr += hdr.len;

	}while( (hdr.len != 0xFFFF) && ( addr < ((uint32_t)eeprom) + FLASH_PAGE_SIZE ) );

	return (n);
}
//----------------------------------------------------------------------------
void eepromCopyPageData(void)
{
uint32_t magic = FLASH_EEPROM_MAGIC;
uint32_t current = eeprom;
uint32_t new = (uint32_t) eep2;
uint32_t addr = eeprom + 4;

eeprom_record_hdr hdr;

uint32_t * data[16];

if(new == current) new = (uint32_t) eep1;

flash_write(new,&magic,sizeof(magic)); // write magic to new page

eeprom = new;

do {
	flash_read(addr, &hdr, sizeof(hdr) );
//	uart_tx("flash_read type=",1);	putdec(hdr.type);

	if( (hdr.type > 0) && ( hdr.len != 0xFFFF ) ){
	flash_read( addr + sizeof(hdr), data, hdr.len - sizeof(hdr) );
	eepromSaveParam( hdr.type, data, hdr.len - sizeof(hdr) );
	}
	addr += hdr.len;

}while( (hdr.len != 0xFFFF) && ( addr < ((uint32_t)current) + FLASH_PAGE_SIZE ) );

eepromErasePage(current);

}
//----------------------------------------------------------------------------
void eepromInit(void)
{
	uint32_t magic = FLASH_EEPROM_MAGIC;
	uint32_t tmp1,tmp2 ;

	eeprom = (uint32_t)eep1;

	flash_read((uint32_t)eep1,&tmp1,sizeof(tmp1));
	flash_read((uint32_t)eep2,&tmp2,sizeof(tmp2));

	if(tmp1 == FLASH_EEPROM_MAGIC && tmp2 == FLASH_EEPROM_EMPTY) {eeprom = (uint32_t)eep1; return;}
	if(tmp2 == FLASH_EEPROM_MAGIC && tmp1 == FLASH_EEPROM_EMPTY) {eeprom = (uint32_t)eep2; return;}

	eepromErasePage((uint32_t)eep1);
	eepromErasePage((uint32_t)eep2);

	eeprom = (uint32_t)eep1;
	flash_write(eeprom,&magic,sizeof(magic));
}
//----------------------------------------------------------------------------
