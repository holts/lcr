#include<reg51.h>		      
#include<RTX51TNY.h>

#include "nx_C51.h"
#include "LCD_PG12864F.h"

//以下不能定义在作务函数里
static volatile BYTE xdata led_h _at_ SEGMENT_LED_H_ADDRESS;
static volatile BYTE xdata led_l _at_ SEGMENT_LED_L_ADDRESS;

static BYTE code BCD[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x82, 0x78, 0x80, 0x90};

static volatile BYTE data s_Counter = 0;

void maintask() _task_ 0
{
    os_create_task(1);
    os_create_task(2);
    os_create_task(3);
    
    os_delete_task(0);
    
}


void led_runing() _task_ 1
{
    BYTE i = 0;
    while(1)
    {
        LED_RUN_ON;
        if(TMO_EVENT == os_wait(K_TMO, 10, 0));
        LED_RUN_OFF;
        if(TMO_EVENT == os_wait(K_TMO, 10, 0));
    }
}


void show_segment_led() _task_ 2
{
    BYTE i = 0;
    while(1)
    {
        led_h = BCD[i / 10];
        led_l = BCD[i % 10];
        i++ ;
        if(i > 99) i = 0;
        if(TMO_EVENT == os_wait(K_TMO, 10, 0));
    }
    
    
//     while(1)
//     {
//         if(TMO_EVENT == os_wait(K_TMO, 10, 0));
//         led_h = BCD[s_Counter / 10];
//         led_l = BCD[s_Counter % 10];
//     }
}

const BYTE code fnt18x18[1][54] =
{
0x00,0x01,0x00,0x1F,0xFF,0x80,0x10,0x01,0x00,0x17,
0xFD,0x00,0x10,0x41,0x00,0x10,0x41,0x00,0x10,0x41,
0x00,0x13,0xF9,0x00,0x10,0x41,0x00,0x10,0x51,0x00,
0x10,0x49,0x00,0x17,0xFD,0x00,0x10,0x01,0x00,0x10,
0x01,0x00,0x1F,0xFF,0x00,0x10,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
};

void lcd_runing() _task_ 3
{
    BYTE i = 0;
    BYTE x, y;
    SBYTE stepX = 1, stepY = 1;

    
    LCD_Init();
    while(1)
    {
        LCD_CLS();
        LCD_RectangleFill(20, 15, 50, 55, _GUI_MODE_CVR);
        for(i = 0; i < 64; ++i)
        {
            LCD_Point(i, i, _GUI_MODE_FLP);
        }
        LCD_Rectangle(5, 5, 123, 59, _GUI_MODE_CVR);
        if(TMO_EVENT == os_wait(K_TMO, 10, 0));
        
        for(i = 0; i < 100; ++i)
        {
            if(x + 18 >= _SCREEN_X_DOTS)
            {
                stepX = -1;
            }
            else if(x == 0)
            {
                stepX = 1;
            }
            
            if(y + 18 >= _SCREEN_Y_DOTS)
            {
                stepY = -1;
            }
            else if(y == 0)
            {
                stepY = 1;
            }
            
            LCD_ShowArray(fnt18x18[0], x, y, 18, 18, _GUI_MODE_FLP);
            if(TMO_EVENT == os_wait(K_TMO, 10, 0));
            LCD_ShowArray(fnt18x18[0], x, y, 18, 18, _GUI_MODE_FLP);
            //if(TMO_EVENT == os_wait(K_TMO, 10, 0));
            
            x += stepX;
            y += stepY;
        }
    }
}

