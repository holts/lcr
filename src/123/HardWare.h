#ifndef _HARDWARE_H
#define _HARDWARE_H

#define LCD_ADDRESS_CMD     0x2FFF
#define LCD_ADDRESS_DAT     0x0FFF

#define SEGMENT_LED_H_ADDRESS   0xBFFF
#define SEGMENT_LED_L_ADDRESS   0x7FFF


#define LED_RUN_ON          _PIN_LO(P1, 6)
#define LED_RUN_OFF         _PIN_HI(P1, 6)

#define LCD_RST_UN  _PIN_HI(P1, 7)
#define LCD_RST_EN  _PIN_LO(P1, 7)

#endif //_HARDWARE_H