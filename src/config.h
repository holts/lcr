#ifndef   CONFIG_H 
#define   CONFIG_H 

/***********************************************************************
*MCU stc12c5a60s2
*晶振 32MHz
*LCD1602 4 位数据线程序
*   端口名称              端口符号    LCD引脚　  MCU端口   
*   RS数据/指令选择线        RS         4         P0^6
*   R/W读写选择线            R/W        5         P0^5
*   EN写使能线               EN         6         P0^4
*   数据总线                 D0-D3     7-10       NC
*                            D4         11        P0^3
*                            D5         12        P0^2
*                            D6         13        P0^1
*                            D7         14        P0^0
*   其他端口连接：
*   电源负                   VSS        1      
*   电源正+4.5--+5.5V        VDD        2
*   对比度调节 (接地最大)    VL         3 
*   背光负                   BLK VSS    15    
*   背光正                   BLA VDD    16
***********************************************************************/

//#define LCD_8BIT
//#define LCD_FUNCTION	0x38	// LCD mode as 8-bit 2-line 5*8-dot 1/16Duty
#define LCD_4BIT
#define LCD_FUNCTION	0x28	// LCD mode as 4-bit 2-line 5*8-dot 1/16Duty

/* LCDPort contains 4-bit data D0 to D3*/
/* LCDPort must be assigned to the chosen port */
#define LCDPort P0

/* Pins E and RS of LCD must be assigned to LCDControlPort*/
#define LCDControlPort P0

/* LCD Enable pin is assigned to Px1 */
#define LCD_Enable_Pin    4
/* LCD R/W pin is assigned to Px5 */
#define LCD_RW_Pin        5
/* LCD RS pin is assigned to Px2 */
#define LCD_RS_Pin        6

#endif
