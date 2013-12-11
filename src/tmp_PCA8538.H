//作者：杜善伍
//日期：2013.08.22

//定义管脚定义、变量、常量
//修改：



#ifndef	PCA8538_H	
#define	PCA8538_H

#define		WRITE_MODE		0x20
#define		READ_MODE		0xa0

#define		LAST_CTRL_BYTE	0x00
#define		CTRL_BYTE_CTN	0x80
#define		Temp_CTRL		0x40


#define		WRITE_RAM_BYTE	0x20

#define		ENABLE			1
#define		DISABLE			0
#define		DISPLAY_ON		1
#define		DISPLAY_OFF		0

#define		DELAY_VALUE		500

#define		SEG_CNT			102
#define		X_SIZE			95
#define		Y_SIZE			2

#define		NUM_START_ADDR	95

unsigned char __xdata LCD_RAM[SEG_CNT];
unsigned char __xdata Current_X_POS = 0x00;


#define		LCD_RST			P3_7
#define 		LCD_CLK			P3_6
#define		LCD_SDAOUT		P3_5
#define		LCD_SDAIN		P3_4
#define 		LCD_CS			P3_3


#define		LED_ON			0
#define		LED_OFF			1

#define		GREEN_LED		P0_0
#define		BLUE_LED			P0_1
#define		RED_LED			P0_2


#define		UART_BUF_SIZE	20//定义串口接收缓冲区大小
unsigned char __xdata UART_RCV_BUF[UART_BUF_SIZE];
unsigned char UART_RCV_CNT = 0x00;

unsigned long UART_OVER_TIME = 0x00000000;
#define		UART_OVER_TIME_VALUE	10000


//定义时间变量
volatile unsigned char __xdata Time_Hour = 0x00;
volatile unsigned char __xdata Time_Minute = 0x00;
volatile unsigned char __xdata Time_Second = 0x00;
volatile unsigned char __xdata Miao_Cnt = 0x00;
volatile unsigned char __xdata Time_Start = DISABLE;
volatile unsigned char __xdata Time_Display = DISABLE;



const unsigned char LCD_Init_COMMAND[] = {//液晶屏初始化序列命令
	
// General control commands
	0x3a,//初始化所有寄存器，恢复到默认值
	0xd8,//OTP-refresh
	0x18,//Device-address
	0xb8,//SYNC1_pin is an output; LCM is a single chip
	0xd4,//Clock-out-ctrl is not available.; 0xd5 is Enable
	0x1c,//Read-select : read temperature;  0x1d is read device status
	0x1f,//Clear-reset-flag

//Read temperatue select
	0x1c,//选择读取温度
	
//Charge pump and LCD bias control commands
	0xd0,// 1/4 bias
	
//Display control commands
	0x00,//Set-MUX-mode,1:9 multiplex drive mode
	
	
	0xff,//END
	
};

//PCA8538支持的帧频频率值
const unsigned int FRAME_RATE_CODE[]={
	45,50,55,60,65,70,75,80,85,90,95,100,
	110,120,130,140,150,160,170,180,190,
	200,210,220,230,240,250,260,270,280,290,300,
};


void Delayms(void){
	unsigned int n = 0x00;
	for(n=250;n>0;n--);
}

void Delays(unsigned int dl){
	unsigned int m = 0x00;
	for(m=dl;m>0;m--){
		Delayms();
	}
	
}

#endif
