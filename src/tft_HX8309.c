/******************************************************************************
 ** File Name:     cstn_pcf8833.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(CSTN)						  *
 ** Author:         Jim zhang                                                 *
 ** DATE:           03/01/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/04/2004     louis.wei	    Create.
 ** 11/15/2005     David.Jia        Modify for GG3728 (HX8309/HYDIS).         *
 ** 11/22/2005     David.Jia        Modify ADC value in Probe; Entersleep code*
 ** 12/09/2005     David.Jia        Implemetation SetDisplayWindow function.  *
 ** 12/12/2005     David.Jia        Himax 8309, copy from HX8309.             *
 ** 12/13/2005     David.Jia        Add InvalidateRectImage to support mpeg4. *
 ** 12/27/2005	   David.Jia		CR373434/37435/37437/37472, set window    *
 **		start point in SetDisplayWindow for HX8309.							  *
 ** 03/03/2006     David.Jia        cr42033, LCD ADC range changed: HX8309,   *
 **     200-230; R61500, 231-249; S1D19105, 250-280.                          *
 ******************************************************************************/

#include "os_api.h"
#include "sci_types.h"
#include "gpio_api.h"
#include "lcd.h"

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#ifdef MAIN_LCD_176_220
#define	HX8309_WIDTH	176
#define	HX8309_HEIGHT	220
#else /* MAIN_LCD_176_220 */
#define	HX8309_WIDTH	128
#define	HX8309_HEIGHT	160
#endif /* MAIN_LCD_176_220 */
#define MP4_MAX_WIDTH	176
#define MP4_MAX_HEIGHT	144

#define OFFSET_X	0

//@David.Jia 2005.11.22
#define ADC_HX8309_LOW    200
#define ADC_HX8309_HIGH   230           //@David.Jia 2006.3.3 cr42033

#undef LCD_USE_DMA
//#define LCD_USE_DMA
extern void dma_init(void);

extern void *LCD_GetLCDBuffer(void);
/******************************************************************************/
//  Description:   Clear color LCD with one color
//  Global resource dependence: 
//  Author:         louis.wei
//  Note:
/******************************************************************************/
LOCAL void HX8309_Clear(
						uint32 color	//color to fill the whole lcd.
						);
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8309_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8309_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/*****************************************************************************/
//  Description:    Get the lcd base information..
//	Global resource dependence: 
//  Author:         louis.wei
//	Note:
/*****************************************************************************/
LOCAL  ERR_LCD_E   HX8309_GetInfo(
								   LCD_INFO_T *lcd_info_ptr	//lcd information struct pointer
								   );

/******************************************************************************/
//  Description:    get the important parameter for digital camera
//	Global resource dependence: 
//  Author:         Zhemin.lin
//	Note:           
/******************************************************************************/
LOCAL ERR_LCD_E  HX8309_GetMainLcdSpec(
	LCD_SPEC_T *spec_ptr 	//spec struct pointer
	);

/*********************************************************************/
//  Description:   Initialize color LCD : HX8309
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E HX8309_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8309_Invalidate(void);

/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
LOCAL ERR_LCD_E HX8309_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	);

/******************************************************************************/
//  Description:  for mp4 display
//	Global resource dependence: 
//  Author:  juan.zhang

/******************************************************************************/
LOCAL ERR_LCD_E HX8309_InvalidateRectImage(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom,	//bottom of the rectangle
	uint16 *buf_ptr,
	uint8  is_invert//ignore
	);

/******************************************************************************/
//  Description:   LCD go to sleep mode
//  Author:         Yingchun.li
//	Note: 
/******************************************************************************/
LOCAL HX8309_GoSleep(void);
/******************************************************************************/
//  Description:   LCD exit sleep mode
//  Author:         Yingchun.li
//	Note: 
/******************************************************************************/
LOCAL HX8309_ExitSleep(void);
/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
extern uint32 dma_request(uint32 chid, uint32 saddr, uint32 daddr, uint32 size, uint32 transfer_mode, uint32 size_mode);

#define HX8309_SEND_COMMAND( c)   		{*(volatile uint16 *)0x58000000 = c;}
	
#define  HX8309_SEND_DATA(d) 	{*(volatile uint16 *)0x58008000 = d;}
 
#define HX8309_sendcommand1(command, data) {*(volatile uint16 *)0x58000000 = command;*(volatile uint16 *)0x58008000 = data;}

#if 0
#define Delayms( ms)\
{\
	int i;\
	for (i=0; i< 100 * ms; i++) ;\
}
#else
//void Delayms (uint32 ms)	
//{
//	uint32 t1, t2;
//	
//	t1 = t2 = SCI_GetTickCount ();
//	
//	do{
//		t2 = SCI_GetTickCount ();
//	}while ((t2-t1) < ms);
//}
	
#endif
//LOCAL void HX8309_HW_reset(void)
//{
//	GPIO_SetLcdReset(0);
//	Delayms(10);
//	GPIO_SetLcdReset(1);
//	Delayms(10);
//}

LOCAL void HX8309_reset(void)
{
    SCI_TRACE_LOW("HX8309_reset: 7370 HX8309.");
    //delay 10ms
	Delayms(10);
	//R10H<-0000H
	HX8309_sendcommand1(0x0010,0x0);
	//R11H<-0000H
	HX8309_sendcommand1(0x0011,0x0);
	//R12H<-0000H
	HX8309_sendcommand1(0x0012,0x0);
	//R13H<-0000H
	HX8309_sendcommand1(0x0013,0x0);
	//R00H<-0001H
	HX8309_sendcommand1(0x0,0x01);
	//delay 30ms
	Delayms(30);

	//R11H<-0001H
	HX8309_sendcommand1(0x0011,0x01);
	//R12H<-0004H
	HX8309_sendcommand1(0x0012,0x04);
	//R13H<-141CH
	HX8309_sendcommand1(0x0013,0x141c);
	//R10H<-0044H
	HX8309_sendcommand1(0x0010,0x0044);
	//R12H<-0018H
	HX8309_sendcommand1(0x0012,0x0018);
	//delay 100ms
	Delayms(100);

	//R10H<-0040H 
	HX8309_sendcommand1(0x0010,0x0040);
	//R13H<-3413H 
	HX8309_sendcommand1(0x0013,0x3413);
	//HX8309_sendcommand1(0x0013,0x3518);
	//R08H<-0202H
	HX8309_sendcommand1(0x0008,0x0202);
	//R09H<-0000H
	HX8309_sendcommand1(0x0009,0x0);
	//R0BH<-0000H
	HX8309_sendcommand1(0x000B,0x0);
	//R21H<-0000H
	HX8309_sendcommand1(0x0021,0x0);
	//R23H<-0000H
	HX8309_sendcommand1(0x0023,0x0);
	//R24H<-0000H
	HX8309_sendcommand1(0x0024,0x0);
	//R30H<-0000H
	HX8309_sendcommand1(0x0030,0x0);
	//R31H<-0000H
	HX8309_sendcommand1(0x0031,0x0);
	//R32H<-0007H
	HX8309_sendcommand1(0x0032,0x07);
	//R33H<-0007H
	HX8309_sendcommand1(0x0033,0x07);
	//R34H<-0000H
	HX8309_sendcommand1(0x0034,0x0);
	//R35H<-0000H
	HX8309_sendcommand1(0x0035,0x0);
	//R36H<-0000H
	HX8309_sendcommand1(0x0036,0x0);
	//R37H<-0000H
	HX8309_sendcommand1(0x0037,0x0);
	//R38H<-0000H
	HX8309_sendcommand1(0x0038,0x0);
	//R39H<-0000H
	HX8309_sendcommand1(0x0039,0x0);
	//R40H<-0000H
	HX8309_sendcommand1(0x0040,0x0);
	//R01H<-011BH
	HX8309_sendcommand1(0x0001,0x011B);
	//R02H<-0700H
	HX8309_sendcommand1(0x0002,0x0700);
	//R03H<-1030H 
	HX8309_sendcommand1(0x0003,0x1030);
	//R04H<-0000H
	HX8309_sendcommand1(0x0004,0x0);
	//R05H<-0000H
	HX8309_sendcommand1(0x0005,0x0);
	//R41H<-0000H
	HX8309_sendcommand1(0x0041,0x0);
	//R42H<-DB00H
	HX8309_sendcommand1(0x0042,0xDB00);
	//R43H<-DB00H
	HX8309_sendcommand1(0x0043,0xDB00);
	//R44H<-AF00H
	HX8309_sendcommand1(0x0044,0xAF00);
	//R45H<-DB00H
	HX8309_sendcommand1(0x0045,0xDB00);
	//R10H<-4040H
	HX8309_sendcommand1(0x0010,0x4040);
	//delay 100ms
	Delayms(100);

	//R07H<-0005H
	HX8309_sendcommand1(0x0007,0x0005);
	//delay 20ms
	Delayms(20);

	//R07H<-0025H
	HX8309_sendcommand1(0x0007,0x0025);
	//R07H<-0027H
	HX8309_sendcommand1(0x0007,0x0027);
	//delay 20ms
	Delayms(20);

	//R07H<-0037H
	HX8309_sendcommand1(0x0007,0x0037);
	//R22H<-Display Data Write
	HX8309_SEND_COMMAND(0x0022);
	    
  /*  HX8309_sendcommand1(0x0001,0x011B);	
	HX8309_sendcommand1(0x0002,0x0700);	
	HX8309_sendcommand1(0x0003,0x6030);     //0x6020	
	HX8309_sendcommand1(0x0004,0x0000);	
	HX8309_sendcommand1(0x0005,0x0000);	
	HX8309_sendcommand1(0x0007,0x0003);		
 	HX8309_sendcommand1(0x0008,0x0202);	
 	HX8309_sendcommand1(0x0009,0x0000);	
	HX8309_sendcommand1(0x000B,0x0004);	
	HX8309_sendcommand1(0x000C,0x0003);	
	HX8309_sendcommand1(0x0040,0x0000);	
	HX8309_sendcommand1(0x0041,0x00EF);	
	HX8309_sendcommand1(0x0042,0xDB00);	
 	HX8309_sendcommand1(0x0043,0xDB00);	
	HX8309_sendcommand1(0x0044,0xAF00);	
 	HX8309_sendcommand1(0x0045,0xDB00);	
	Delayms(100);

//############# void Power_Set(void) ################//

	HX8309_sendcommand1(0x0000,0x0001);	
	Delayms(1);	         	 
	HX8309_sendcommand1(0x0010,0x0000);	
	HX8309_sendcommand1(0x0011,0x0000);	
	HX8309_sendcommand1(0x0012,0x0000);	
	HX8309_sendcommand1(0x0013,0x0000);	         
	HX8309_sendcommand1(0x0011,0x0000);	
	HX8309_sendcommand1(0x0013,0x1517);	
	HX8309_sendcommand1(0x0012,0x0008);	 
	HX8309_sendcommand1(0x0010,0x4040);	
	Delayms(1);			 
 	HX8309_sendcommand1(0x0011,0x0000);	
	HX8309_sendcommand1(0x0010,0x0054);			
	HX8309_sendcommand1(0x0012,0x0014);	
	Delayms(10);	         			
	HX8309_sendcommand1(0x0010,0x4040);	
	Delayms(10);	 
	HX8309_sendcommand1(0x0013,0x3413);     //0x3A1B	 
	Delayms(100);


//################# void Gamma_Set(void) ####################//  
 
	HX8309_sendcommand1(0x0030,0x0406);	
	HX8309_sendcommand1(0x0031,0x0306);	
	HX8309_sendcommand1(0x0032,0x0000);	
	HX8309_sendcommand1(0x0033,0x0003);	
	HX8309_sendcommand1(0x0034,0x0406);	
	HX8309_sendcommand1(0x0035,0x0306);	
	HX8309_sendcommand1(0x0036,0x0000);	
	HX8309_sendcommand1(0x0037,0x0003);	
	HX8309_sendcommand1(0x0038,0x0000);	
	HX8309_sendcommand1(0x0039,0x0000);	
					
//################# void Gamma_Set(void) ####################//

	HX8309_sendcommand1(0x0030,0x0300);	
	HX8309_sendcommand1(0x0031,0x0001);	
	HX8309_sendcommand1(0x0032,0x0700);	
	HX8309_sendcommand1(0x0033,0x0100);	 
	HX8309_sendcommand1(0x0034,0x0700);	
	HX8309_sendcommand1(0x0035,0x0607);	
	HX8309_sendcommand1(0x0036,0x0704);	
	HX8309_sendcommand1(0x0037,0x0202);	
	HX8309_sendcommand1(0x0038,0x0006);	
	HX8309_sendcommand1(0x0039,0x0404);	
				
//################## void Display_ON(void) ####################//

	HX8309_sendcommand1(0x0007,0x0045);					
	Delayms(40);  
	HX8309_sendcommand1(0x0007,0x0065);												
	HX8309_sendcommand1(0x0007,0x0067);	
	Delayms(40);			
    HX8309_sendcommand1(0x0007,0x0077);	
	Delayms(10);			
	HX8309_SEND_COMMAND(0x0022);  // Set to index R22h for transmission data to SRAM.
*/	
}

  /******************************************************************************/
//  Description:   Clear color LCD with one color
//	Global resource dependence: 
//  Author:         louis.wei
//	Note:
 //  modify:  jim.cui	2005.0728  use dma to transport data  
/******************************************************************************/
LOCAL void HX8309_Clear(
						uint32 color	//color to fill the whole lcd.
						)
{
	uint32 i;
	uint16 *buf_ptr = (uint16 *)LCD_GetLCDBuffer();

	uint16  fill_color = color;
	
	for(i=0; i<(HX8309_HEIGHT ); i++)
	{
    #ifdef LCD_USE_DMA
            dma_request(0, (uint32)&fill_color, (uint32)(buf_ptr + i * HX8309_WIDTH), HX8309_WIDTH, 
                                     1, 1);
	#else
	    {
	        int j;
	        for (j = 0; j < HX8309_WIDTH; j++)
	        {
	            HX8309_SEND_DATA(*(buf_ptr+i*HX8309_WIDTH+j));
	        }	
	    }
	#endif
	}

	HX8309_Invalidate();
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
//  modify:  jim.cui  2005.0728  use  dma to transport data  
/******************************************************************************/
LOCAL ERR_LCD_E HX8309_Invalidate(void)
{
	uint16 i,j;
	uint16 address = 0;
	uint16 *buf_ptr = (uint16 *)LCD_GetLCDBuffer();
	
#ifdef LCD_USE_DMA	
	uint32  dummy_before,dummy_after;

	
	address = (uint16) ( (((HX8309_WIDTH - 1 + OFFSET_X) & 0xFF ) << 8 ) | (OFFSET_X));
	HX8309_sendcommand1(0x44, address);		// set horizon address

	address = (uint16) ( ((HX8309_HEIGHT-1) & 0xFF) << 8 );
	HX8309_sendcommand1(0x45, address); 	//  set vertical  address
	
	
	HX8309_sendcommand1(0x0021, OFFSET_X);	

	HX8309_SEND_COMMAND(0x0022); 			// send data.

	dummy_before =  1 % 4;		// Inserted numbers befor every arrow.
	dummy_after =  3 - (HX8309_WIDTH % 4);	// Inserted numbers after every arrow.
        
	for(j=0; j<(HX8309_HEIGHT ); j++)
	{

            dma_request(0, (uint32)(buf_ptr + j * HX8309_WIDTH), 0x58008000, HX8309_WIDTH, 
                                     1, 0);
		
	}

#else
	address = (uint16) ( (((HX8309_WIDTH - 1 + OFFSET_X) & 0xFF ) << 8 ) | (OFFSET_X));	
	HX8309_sendcommand1(0x16, address);		// set horizon address

	address = (uint16) ( ((HX8309_HEIGHT-1) & 0xFF) << 8 );
	HX8309_sendcommand1(0x17, address); 	//  set vertical  address 

	// Set start RAM address (AC register)
	
	HX8309_sendcommand1(0x21, OFFSET_X);	

	HX8309_SEND_COMMAND(0x0022); 			// send data.
	
	for(i=0; i<(HX8309_WIDTH * HX8309_HEIGHT); i++)
		HX8309_SEND_DATA( *buf_ptr++ );
		
#endif
		
	return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:    Get the lcd base information..
//	Global resource dependence: 
//  Author:         louis.wei
//	Note:
/*****************************************************************************/
LOCAL  ERR_LCD_E   HX8309_GetInfo(
								   LCD_INFO_T *lcd_info_ptr	//lcd information struct pointer
								   )
{
	if ( PNULL == lcd_info_ptr )
	{
		return ERR_LCD_POINTER_NULL;	
	}
	
	lcd_info_ptr->r_bitmask			= 0xf800;
	lcd_info_ptr->g_bitmask			= 0x07e0;
	lcd_info_ptr->b_bitmask			= 0x001f;
	lcd_info_ptr->bits_per_pixel	= 16;
	lcd_info_ptr->contrast_min		= 0x00;
	lcd_info_ptr->contrast_max   	= 63;
	lcd_info_ptr->contrast_defaut	= 0x0A;
	
	lcd_info_ptr->lcd_width		= HX8309_WIDTH;
	lcd_info_ptr->lcd_height	= HX8309_HEIGHT;
	lcd_info_ptr->lcdbuff_ptr	= (void *)LCD_GetLCDBuffer();
		
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void HX8309_set_display_window(
	uint8 left, 	// start Horizon address
	uint8 top, 		// start Vertical address
	uint8 right, 	// end Horizon address
	uint8 bottom	// end Vertical address
	)
{
	uint16 address = 0;

	address = (uint16) ((((right & 0xFF) + OFFSET_X)<<8) | ((left & 0xFF) + OFFSET_X));
	HX8309_sendcommand1(0x44, address);		// set horizon address

	address = (uint16) (((bottom & 0xFF)<<8) | (top & 0xFF));
	HX8309_sendcommand1(0x45, address); 	//  set vertical  address position
}


/******************************************************************************/
//  Description:  Set start RAM address which is write to AC(Address
//                Counter) register.
//  Input:
//      left: start Horizon address of AC
//      top: start Vertical address of AC.
//  Return:
//      None.
//	Note:           
/******************************************************************************/
__inline void HX8309_set_start_address(
	uint8 left, 
	uint8 top
	)
{
	uint16 address = 0;
		
	address = (uint16) (((top & 0xFF)<<8) | ((left & 0xFF) + OFFSET_X));
	HX8309_sendcommand1(0x21, address);	// Set start RAM address (AC register)
}

/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
//  modify:  jim.cui  2005.0728  use  dma to transport data  
/******************************************************************************/

LOCAL ERR_LCD_E HX8309_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
	uint32 i, j;
	//uint32 dummy_before, dummy_after;
	uint32 row, column,rect_width;
	uint16 *buf_ptr = (uint16 *)LCD_GetLCDBuffer();
	

  	left 	= (left >= HX8309_WIDTH)    ? HX8309_WIDTH-1 : left;
	right 	= (right >= HX8309_WIDTH)   ? HX8309_WIDTH-1 : right;
	top 	= (top >= HX8309_HEIGHT)    ? HX8309_HEIGHT-1 : top;
	bottom 	= (bottom >= HX8309_HEIGHT) ? HX8309_HEIGHT-1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		return ERR_LCD_PARAMETER_WRONG;
	}
 
 	 	
	HX8309_set_display_window(left, top, right, bottom);
	
	// In High Speed RAM Write Mode. Maybe some dummy data are insterted.
	//dummy_before =  (left) % 4;		// Inserted numbers befor every arrow.
	//dummy_after =  3 - (right % 4);	// Inserted numbers after every arrow.
	
#ifdef LCD_USE_DMA 		
	HX8309_set_start_address(left, top);

	HX8309_SEND_COMMAND(0x22); 			// send data.
       rect_width = right-left+1;
       
	for (j = top; j <= bottom; j++)
	{	

            dma_request(0, (uint32)(buf_ptr + j * HX8309_WIDTH+left), 0x58008000, rect_width, 
                    1, 0);	
	
	}
#else

	row = bottom - top;
	column = right - left;

	HX8309_set_start_address(left, top);

	HX8309_SEND_COMMAND(0x22); 			// send data.

	for (i = 0; i <= row; i++)
	{	
		//for(j = 0; j<dummy_before; j++) // Insert dummy write befor real write.
			//HX8309_SEND_DATA(0x00);
			
		for (j = 0; j <= column; j++)	// real write 
			HX8309_SEND_DATA( *(buf_ptr + (top+i)*HX8309_WIDTH + left+j) );
	
		//for(j=0; j<dummy_after; j++)	// Insert dummy write after real write.
			//HX8309_SEND_DATA(0x00);
	}			
#endif
	
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:  for mp4 display
//	Global resource dependence: 
//  Author:  juan.zhang

/******************************************************************************/
LOCAL ERR_LCD_E HX8309_InvalidateRectImage(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom,	//bottom of the rectangle
	uint16 *buf_ptr,
	uint8  is_invert//ignore
	)
{
	uint32 i, j;
	//uint32 dummy_before, dummy_after;
	uint32 row, column,rect_width;

  	left 	= (left >= MP4_MAX_WIDTH)    ? MP4_MAX_WIDTH-1 : left;
	right 	= (right >= MP4_MAX_WIDTH)   ? MP4_MAX_WIDTH-1 : right;
	top 	= (top >= MP4_MAX_HEIGHT)    ? MP4_MAX_HEIGHT-1 : top;
	bottom 	= (bottom >= MP4_MAX_HEIGHT) ? MP4_MAX_HEIGHT-1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		return ERR_LCD_PARAMETER_WRONG;
	}
 
 	 	
	HX8309_set_display_window(left, top, right, bottom);
	
	// In High Speed RAM Write Mode. Maybe some dummy data are insterted.
	//dummy_before =  (left) % 4;		// Inserted numbers befor every arrow.
	//dummy_after =  3 - (right % 4);	// Inserted numbers after every arrow.
	
#ifdef LCD_USE_DMA 		
	HX8309_set_start_address(left, top);

	HX8309_SEND_COMMAND(0x22); 			// send data.
       rect_width = right-left+1;
       
	for (j = top; j <= bottom; j++)
	{	

            dma_request(0, (uint32)(buf_ptr + j * rect_width +left), 0x58008000, rect_width, 
                    1, 0);	
	
	}
#else

	row = bottom - top;
	column = right - left;

	HX8309_set_start_address(left, top);

	HX8309_SEND_COMMAND(0x22); 			// send data.

	for (i = 0; i <= row; i++)
	{	
		//for(j = 0; j<dummy_before; j++) // Insert dummy write befor real write.
			//R61500_SEND_DATA(0x00);
			
		for (j = 0; j <= column; j++)	// real write 
			HX8309_SEND_DATA( *(buf_ptr + (top+i)*HX8309_WIDTH + left+j) );
	
		//for(j=0; j<dummy_after; j++)	// Insert dummy write after real write.
			//R61500_SEND_DATA(0x00);
	}			
#endif
	
	return ERR_LCD_NONE;
}

/*@Zhemin.Lin, CR9122, begin*/

static LCD_SPEC_T g_HX8309_spec =
{
	0,		//uint8	rgb_sequence; 		/*rgb sequence*/
										/*0: R-G-B, 1: B-G-R*/
	10,		//uint8	min_cycle_read;		/*read cycle:  ns*/
	50,		//uint8	min_cycle_write;	/*write cycle: ns*/
	0,		//uint8	cyclenum_sendaddr;	/*operation cycle to send address*/
										/* 0: once, 1:twice*/
	0,		//uint8	cyclenum_senddata;	/*operation cycle to send data */
										/* 0; once, 1:twice*/
	1,		//uint8	cmd_num_setupwin;	/*command number to settint up widnow space*/
										/*0: 4 commands for setting up window space: x-start, x-end, y-start, y-end*/
										/*1: 2 commands for setting up window space: x-address, y-address*/
										/*2: 1 commanns for setting up window space window address*/
										/*3: no commands for setting up window space*/
	1,		//uint8	method_send_cmdaddr;/*how to send command & address*/
										/*0: sending together, 1:sending separately*/
	1,		//uint8	ads_is_high;		/*status of ADS when sending parameter*/
										/*0: low, 1: high*/
	0,		//uint8	sequence_cmdaddr;	/*the sequence of command & address, if not send command & address together as 16 bits*/
										/*0: write 2 window area on a 16bit bus, Hitachi*/
										/*1, write 1 sindow area with one command, Casio*/
										/*2, write 1 command and 2 parameters separately, Samsung*/
										/*3, reserved*/
										/*4, write 1 command, 2 parameters for page, 4 parameters for column, Epson*/
										/*5, write 1 command, 4 parameters for page, 2 parameters for column*/
										/*6, write 1 command, write 4 parameters for each page/column*/
	4,		//uint8	cyclenum_setupwin;	/*operation cycle to set up window*/
	1,		//uint8	method_gram_access;	/*select method to begin actul data read/write address for selected window area in GRAM*/
										/*0: without any assigning x/y address, use initial start address of window*/
										/*1: assign 1 x/y set command and parameter onto 16bit bus at onece*/
										/*2: use separate for X and Y, with upper byte consists of command and lower byte for parameter*/
										/*3, 1 command for selecting X/Y adn 2 parameters, us command and parameter separately*/
										/*4, set different command for x and y, and command and parameter differs*/
	1,		//uint8	sequence_gram_access;/*read/write command and sequence for GRAM*/
										/*0: perform read/write without seperate read/write command but by strobe 'high' ADS(RS)pin, samsung*/
										/*1: data cycle comes right afer read/write command, Toshiba/Matsushita/Hitachi/NEC*/
										/*2: after read/write command follows start address parameter for X and Y and follows data cycle, Casio*/
										/*3: similiar to 2 but sends X and Y parameters together on 16 bit BUS*/
	0,		//uint8	order_xy;			/*order of x and y address, I think, this field is active when sequence_gram_access = 2*/
										/*0: X first*/
										/*1: Y first*/
	1,		//uint8	is_dummy_read;		/*deciding first read data when reading GRAM*/
										/*0: acknowledge first data as valid data*/
										/*1: acknowledge first data as dummy data*/
	3,		//uint8	cyclenum_read;		/*total cycle numbers needed to access GRAM read/write*/
	0x44,	//uint8	win_setstartx_cmd;	/*window x start address set command*/
	0x00,	//uint8	win_setendx_cmd;	/*window x end address set command*/
	0x45,	//uint8	win_setstarty_cmd;	/*window y start address set command*/
	0x00,	//uint8	win_setendy_cmd;	/*window y end address set command*/
	0x21,	//uint8	gram_setx_cmd;		/*gram x address set command*/
	0x00,	//uint8	gram_sety_cmd;		/*gram y address set command*/
	0x22,	//uint8	gram_read_cmd;		/*gram read command*/
	0x22,	//uint8	gram_write_cmd;		/*gram write command*/
	//@zhemin.lin, add 1 line, CR9590
	0x00,	//uint8	line_offset;		/*line offset for display on lcd*/
	0x00	//uint8 colum_offset        /*columoffset for display on lcd*/
};


/******************************************************************************/
//  Description:    get the important parameter for digital camera
//	Global resource dependence: 
//  Author:         Zhemin.lin
//	Note:           
/******************************************************************************/
LOCAL ERR_LCD_E  HX8309_GetMainLcdSpec(
	LCD_SPEC_T *spec_ptr 	//spec struct pointer
	)
{
	if (spec_ptr == PNULL)
	{
		return 1;
	}
	
	memcpy(spec_ptr, &g_HX8309_spec, sizeof(LCD_SPEC_T));
	
	return ERR_LCD_NONE;
}


/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8309_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	SCI_TRACE_LOW("HX8309_EnterSleep,%d", is_sleep);
	
	
	if ( is_sleep ) // enter sleep mode.
	{

		//HX8309_sendcommand1(0x0007, 0x0000);	//DTE=0, D1-0=0, GON=0
		//HX8309_sendcommand1(0x0003, 0x0002);	//SLP=1
		
        	//Delayms(40);        
        	HX8309_GoSleep();
        
		
	}
	else 			// out sleep mode 
	{
		HX8309_reset();
		//HX8309_ExitSleep();
		HX8309_Invalidate();
	}

	/*
	if( !GetUdiskState())
		DC_EnterSleep(is_sleep);
	*/
	return ERR_LCD_NONE;
}


/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E HX8309_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
    HX8309_set_display_window(left, top, right, bottom);
    //@David.Jia	2005.12.27 cr37434 begin
    HX8309_set_start_address(left, top);
    //@David.Jia	2005.12.27 cr37434 end
    HX8309_SEND_COMMAND(0x0022);
    
	return ERR_LCD_NONE;
}

/*********************************************************************/
//  Description:   Initialize color LCD : HX8309
//  Input:
//      None.
//  Author:         yinchun.li
//  Return:
//      None.
//	Note:
//  modify:  jim.cui  2005.0728 add dma init  
/*********************************************************************/
LOCAL ERR_LCD_E HX8309_Init(void)
{

       //dma init
    #ifdef LCD_USE_DMA   
       dma_init ();
    #endif 

 	//HX8309_HW_reset();
   	HX8309_reset();

	HX8309_Clear( 0xf25f );

	
	return ERR_LCD_NONE;
}


/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   HX8309_SetContrast(
	uint16  contrast	//contrast value to set
	)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
} 


/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   HX8309_SetBrightness(
	uint16 brightness	//birghtness to set
	)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8309_Close()
{
	GPIO_SetLcdBackLight( SCI_FALSE );
    HX8309_EnterSleep( SCI_TRUE );	
}


LOCAL LCD_OPERATIONS_T HX8309_operations = 
{
	HX8309_Init,
	HX8309_EnterSleep,
	HX8309_SetContrast,
	HX8309_SetBrightness,
	HX8309_SetDisplayWindow,
	HX8309_GetInfo,
	HX8309_InvalidateRect,
	HX8309_InvalidateRectImage,
	HX8309_Invalidate,
	HX8309_Clear,
	HX8309_Close,
	HX8309_GetMainLcdSpec
};

/******************************************************************************/
//  Description:  return the HX8309 lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* HX8309_GetOperations()
{
	return &HX8309_operations;
}

#define ADC_NUM 10
PUBLIC BOOLEAN HX8309_Probe(void)
{
    uint32 i, adc_val = 0, adc_sum = 0;
	
	/*Delayms(500);
		
	for(i=0; i<ADC_NUM; i++)
	{
	    adc_val = ADC_GetResultDirectly(ADIN_1, FALSE);
	    adc_sum += adc_val;
	    SCI_TRACE_LOW("adc_rlt[%d]: %d", i, adc_val);
	    Delayms(50);
	}
	
	adc_val = adc_sum / ADC_NUM;*/
	
	adc_val = LCD_GetADCValue(ADIN_1);//added by lipengyu.
	SCI_TRACE_LOW("average adc_val: %d", adc_val);
	
	if((adc_val >= ADC_HX8309_LOW) && (adc_val <= ADC_HX8309_HIGH))        //Á¦Ê¤ 8309
	{
	    return SCI_TRUE;	
	}
	
	return SCI_FALSE;
}

#if 0
/*-----------------------------------------------------------------------------------------------
	Function name	: mv3LcdCmdWrite()
	Prototype		: void mv3LcdCmdWrite(mvUint16 addr, mvUint16 data);
	Return		: void
	Argument		: addr -> address
				  data -> data (This data is not pixel data but is register setting value)
	Comments	: command from MCU into LCD 
				  Sending a command to LCD, you should be change the RS pin by low value.
-----------------------------------------------------------------------------------------------*/
void mv3LcdCmdWrite(mvUint16 addr)
{
	mv3LcdClearRS();
	mv3LcdDataWrite(addr);
	mv3LcdSetRS();
}



/*-----------------------------------------------------------------------------------------------
	Function name	: mv3WriteDataToMainLcd()
	Prototype		: void mv3WriteDataToMainLcd(mvUint32 data);
	Return		: void
	Argument		: data -> data 
	Comments	: data from MCU into LCD 
-----------------------------------------------------------------------------------------------*/
void mv3WriteDataToMainLcd(mvUint32 data)
{
#ifdef COLOR18
	mv3LcdDataWrite(data >> 2);
	mv3LcdDataWrite(data << 14);
#else
	mv3LcdDataWrite(data);
#endif
}


/*-----------------------------------------------------------------------------------------------
	Function name	: mv3SetMainLcdWindow()
	Prototype		: void mv3SetMainLcdWindow();
	Return      	: void
	Argument      	: None
	Comments   	: set coordinates to display LCD
-----------------------------------------------------------------------------------------------*/
void mv3SetMainLcdWindow(mvUint16 sax, mvUint16 say, mvUint16 eax, mvUint16 eay)
{
	mvUint16 addr;

	mv3LcdBankSelect();
	
	addr = (( eax+2) << 8 );
	addr |= ( sax+2 );                                   //changed by wang 2005.01.21

	mv3LcdCmdWrite( 0x16 );
	mv3WriteDataToMainLcd( addr );

	addr = ( eay << 8 );
	addr |= ( say );

	mv3LcdCmdWrite( 0x17 );
	mv3WriteDataToMainLcd( addr );

	addr = ( say << 8 );
	addr |= ( sax );

	mv3LcdCmdWrite( 0x21 );
	mv3WriteDataToMainLcd( addr );
	
}



/*-----------------------------------------------------------------------------------------------
	Function name	: mv3SetMainLcdStart()
	Prototype   	: void mv3SetMainLcdStart();
	Return        	: void
	Argument     	: None
	Comments    	: set start position to display LCD
-----------------------------------------------------------------------------------------------*/
void mv3SetMainLcdStart(mvUint16 sax, mvUint16 say)
{
	mv3LcdBankSelect();
	mv3LcdCmdWrite(0x21);
	mv3WriteDataToMainLcd((say << 8) + sax );
	mv3LcdCmdWrite(0x22);
}



/*-----------------------------------------------------------------------------------------------
	Function name	: mv3MainLcdInit()
	Prototype   	: void mv3MainLcdInit(mvUint16 sax, mvUint16 say);
	Return       	: void
	Argument     	: None
	Comments     	: LCD Setting for MV3
-----------------------------------------------------------------------------------------------*/
void mv3MainLcdInit(mvUint16 sax, mvUint16 say)
{
	mvUint16 lcdCommand[10][3] = {
#if defined(FEATURE_MV3X7)
		// WE = write enable,  RS = register set
		// When RS is 0, command is excuted.
		// when RS is 1, data is transmitted.
		// {WE, RS, DATA}
		// before preview, LCD 4 word command
		{MV3_LCD_WRITE_ENABLE, MV3_LCD_RS_CLS, 0x21},
		{MV3_LCD_WRITE_ENABLE, MV3_LCD_RS_SET, 0},
		{MV3_LCD_WRITE_ENABLE, MV3_LCD_RS_CLS, 0x22},
		{0, 0, 0},
		// After preview, LCD 4 word command
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},
		// reserved
		{0, 0, 0},
		{0, 0, 0}
#elif defined(FEATURE_MV3X9)
		//Before preview, LCD 5 word command
		{MV3_LCD_WRITE_ENABLE, MV3_LCD_RS_CLS, 0x21},
		{MV3_LCD_WRITE_ENABLE, MV3_LCD_RS_SET, 0},
		{MV3_LCD_WRITE_ENABLE, MV3_LCD_RS_CLS, 0x22},
		{0, 0, 0},
		{0, 0, 0},
		// After preview, LCD 5 word command
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
#endif
    };
	lcdCommand[1][2] = (say << 8) + (sax+2);                //changed by wang 2005.01.21

	mv3LcdCmdControl((mvUint16 * )lcdCommand);
}



/*-----------------------------------------------------------------------------------------------
	Function name	: mv3MainLcdDisplayRegion()
	Prototype    	: void mv3MainLcdDisplayRegion();
	Return       	: void
	Argument     	: none
	Comments     	: write gamma table
-----------------------------------------------------------------------------------------------*/
void mv3MainLcdDisplayRegion(mvUint32 sax, mvUint32 say, mvUint32 eax, mvUint32 eay,
                             mvUint16 *pFullImage)
{
	mvUint16 i, j;

	// set window 
	mv3SetMainLcdWindow(sax, say, eax, eay);
	mv3SetMainLcdStart(sax, say);

	pFullImage += say * MAX_M_LCD_X + sax;
	for(i = 0; i < (eay - say + 1); i++)
	{
		for(j = 0; j < (eax - sax + 1); j++)
		{
			mv3WriteDataToMainLcd((mvUint32)*(pFullImage + j));
		}
		pFullImage += MAX_M_LCD_X;
	}

}
#endif
LOCAL void HX8309_DisplayOn(void)
{
//@David.Jia 2005.11.22 begin
//	HX8309_sendcommand1(0x0007, 0x0001);	
//	Delayms(30); 
//	HX8309_sendcommand1(0x0007, 0x0025);	
//	HX8309_sendcommand1(0x0007, 0x0027);
//	Delayms(30); 
//	HX8309_sendcommand1(0x0007, 0x0037);
    HX8309_sendcommand1(0x0007, 0x0045);	
	Delayms(30); 
	HX8309_sendcommand1(0x0007, 0x0065);	
	HX8309_sendcommand1(0x0007, 0x0063);
	Delayms(30); 
	HX8309_sendcommand1(0x0007, 0x0067);
//@David.Jia 2005.11.22 end
}
LOCAL void HX8309_DisplayOff(void)
{
	HX8309_sendcommand1(0x0007, 0x0036);	
	Delayms(30); 
	HX8309_sendcommand1(0x0007, 0x0026);
	Delayms(30); 
	HX8309_sendcommand1(0x0007, 0x0004);

	//power off
	HX8309_sendcommand1(0x0010, 0x0000);	
	HX8309_sendcommand1(0x0012, 0x0000);
	HX8309_sendcommand1(0x0013, 0x0000);
}

LOCAL HX8309_GoSleep(void)
{
	HX8309_DisplayOff();
	HX8309_sendcommand1(0x0010, 0x0001);	
}

LOCAL HX8309_ExitSleep(void)
{
//@David.Jia 2005.11.22 begin
//	HX8309_sendcommand1(0x0010, 0x0000);
//	// power setting
//	  HX8309_sendcommand1(0x0012,0x0000);
//	   Delayms(40);   
//	  HX8309_sendcommand1(0x0013,0x0000);
//	  Delayms(40);   //delay
//	  // initfun 2
//	  HX8309_sendcommand1(0x0011,0x0003);
//	  Delayms(40);   //delay
//	   
//	  HX8309_sendcommand1(0x0012,0x0000);
//	   Delayms(40); 
//
//	  HX8309_sendcommand1(0x0013,0x0000);
//	  Delayms(40);
//	  HX8309_sendcommand1(0x0010,0x0000);
//	  Delayms(40);
//	  HX8309_sendcommand1(0x0010,0x0044);
//	  Delayms(40);
//	  //HX8309_sendcommand1(0x000b,0x0400);
//	  //Delayms(40);
//	  HX8309_sendcommand1(0x0012,0x001e);
//	  Delayms(40);
//	  HX8309_sendcommand1(0x0013,0x351e);
//	  Delayms(40);
//	  HX8309_sendcommand1(0x0010,0x0040);
//	  Delayms(40);
//	  HX8309_sendcommand1(0x0010,0x6360);
      HX8309_sendcommand1(0x0010, 0x0000);
      // power setting
      HX8309_sendcommand1(0x0010, 0x0044);
      HX8309_sendcommand1(0x0011, 0x0000);
      HX8309_sendcommand1(0x0012, 0x0008); 
      HX8309_sendcommand1(0x0013, 0x0410);
      Delayms(50);
      HX8309_sendcommand1(0x0012, 0x0018);
      Delayms(50);
      
      HX8309_sendcommand1(0x0010, 0x2360);
      Delayms(50);
        
      HX8309_sendcommand1(0x0013, 0x2413);
      Delayms(100);
//@David.Jia 2005.11.22 begin     
	HX8309_DisplayOn();
	  
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

