#include "nx_C51.h"
#include "T6963.h"
#include "LCD_PG12864F.h"


static volatile BYTE xdata lcd_cmd _at_ LCD_ADDRESS_CMD;
static volatile BYTE xdata lcd_data _at_ LCD_ADDRESS_DAT;

#define CLI(x)
#define SEI(x)


//////////////////////////////////////////////////////////////////////////
void T6963_InitBus(void)
{
}

//////////////////////////////////////////////////////////////////////////
BYTE T6963_StatusCheck(BYTE mask)
{
    WORD data t = 0;
    #define _STATUS_WAIT_MAX        (1000)

    while((lcd_cmd & mask) != mask && t < _STATUS_WAIT_MAX);
    
    return t < _STATUS_WAIT_MAX;
}

//////////////////////////////////////////////////////////////////////////
void T6963_CMD(BYTE x)
{
    CLI();

    T6963_StatusCheck(_STATE_CHK_RW);
    
    lcd_cmd = x;

    SEI();
}

//////////////////////////////////////////////////////////////////////////
void T6963_WR(BYTE x)
{
    CLI();
       
    T6963_StatusCheck(_STATE_CHK_RW);

    lcd_data = x;
    
    SEI();
}

//////////////////////////////////////////////////////////////////////////
BYTE T6963_RD(void)
{
    BYTE x = 0;
    
    CLI();
    
    T6963_StatusCheck(_STATE_CHK_RW);

    x = lcd_data;

    SEI();
    
    return x;    
}


//////////////////////////////////////////////////////////////////////////
void T6963_AutoWR(BYTE x)
{
    CLI();
       
    T6963_StatusCheck(_STATE_CHK_AUTO_WR);

    lcd_data = x;
    
    SEI();
}

//////////////////////////////////////////////////////////////////////////
BYTE T6963_AutoRD(void)
{
    BYTE data x = 0;
    
    CLI();
    
    T6963_StatusCheck(_STATE_CHK_AUTO_RD);

    x = lcd_data;

    SEI();
    
    return x;    
}

//////////////////////////////////////////////////////////////////////////
BYTE T6963_ReadData(enum_Dir dir)
{
    BYTE data cmd = _CMD_DATA;
    
    if(dir == No_move)
    {
        cmd |= _CMD_DATA_RD_NO_MOVE;
    }
    else if(dir == Increase)
    {
        cmd |= _CMD_DATA_RD_INCREASE;
    }
    else
    {
        cmd |= _CMD_DATA_RD_DECREASE;
    }
    
    T6963_CMD(cmd);
    cmd = T6963_RD();
    
    return cmd;
}

void T6963_WriteData(BYTE dat, enum_Dir dir)
{
    BYTE cmd = _CMD_DATA;
    
    T6963_WR(dat);
    
    if(dir == No_move)
    {
        cmd |= _CMD_DATA_WR_NO_MOVE;
    }
    else if(dir == Increase)
    {
        cmd |= _CMD_DATA_WR_INCREASE;
    }
    else
    {
        cmd |= _CMD_DATA_WR_DECREASE;
    }
    
    T6963_CMD(cmd);
}



