/*
实现了PCA8538液晶模块的底层驱动程序， 基本的初始化，操作指定位置的像素； 显示字符、字符串； 基本的底层参数设置，如：偏压设置、VOP设置、帧频设置等； 

实现了读取模块温度，并计算出当前温度值；
*/

#include "PCA8538.H"	

/*******************************************************************************
*函  数: dly                                                                   *
*功  能: 延时                                                                  *
*参  数: 无                                                                    *
*******************************************************************************/ 
void dly(void)
{
	unsigned char n = 0x00;
	for(n=0;n<2;n++);
}

/*******************************************************************************
*函  数: SPI_Send_Byte                                                         *
*功  能: SPI 发送一个字节                                                      *
*参  数: 需要传送的Ascii                                                       *
*******************************************************************************/ 
void SPI_Send_Byte(unsigned char dat)
{
	unsigned char n = 0x00;
	
	for(n=0;n<8;n++){
		LCD_CLK = 0;
		//dly();
		if((dat & 0x80) == 0x80){
			LCD_SDAIN = 1;
		}else{
			LCD_SDAIN = 0;
		}
		//dly();
		LCD_CLK = 1;
		//dly();
		dat = dat << 1;
	}	
}

/*******************************************************************************
*函  数: SPI_Write                                                             *
*功  能: 向LCD写命令或者数据 ，先写地址，再写命令，然后写参数                  *
*参  数: 地址，命令，参数值                                                    *
*******************************************************************************/ 
void SPI_Write(unsigned char sub_addr,unsigned char cmd_dat,unsigned char para)
{
	LCD_CLK = 0;
	LCD_CS = 0;
	
	SPI_Send_Byte(sub_addr);
	SPI_Send_Byte(cmd_dat);
        SPI_Send_Byte(para);
	
	LCD_CS = 1;
}

/*******************************************************************************
*函  数: Set_Charge_Pump                                                       *
*功  能: 设置打开/关闭倍压功能，设置倍压值                                     *
*参  数: 开/关,倍压值                                                          *
*******************************************************************************/ 
void Set_Charge_Pump(unsigned char enbl,unsigned char XX)
{
	if(enbl == DISABLE){
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xc0));
	}else{
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xc8 | ((XX & 0x07) - 2)));
	}
}

/*******************************************************************************
*函  数: Set_VLCD                                                              *
*功  能: 设置液晶屏驱动电压                                                    *
*参  数: 需要的电压值                                                          *
*******************************************************************************/ 
void Set_VLCD(unsigned int value)
{
	unsigned int tmp = 0x0000;
	unsigned char dat = 0x00;
	
	value = value & 0x01ff; //总共9位设置值
	
	tmp = value;
	tmp = tmp & 0xfff0;
	tmp = tmp >> 4;
	dat = (unsigned char)(tmp);
	
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0x40 | dat));
	
	tmp = value;
	tmp = tmp & 0x000f;
	dat = (unsigned char)(tmp);
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0x60 | dat));
}

/*******************************************************************************
*函  数: Set_Inversion_Mode                                                    *
*功  能: 设置                                                                  *
*参  数: 模式                                                       *
*******************************************************************************/ 
void Set_Inversion_Mode(unsigned char mode)
{ SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xb0 | (mode & 0x07))); }

/*******************************************************************************
*函  数: Set_Display_Mode                                                      *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Set_Display_Mode(unsigned char mode)
{
	if(mode == DISPLAY_ON){
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0x39);
	}else{
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0x38);
	}
}

/*******************************************************************************
*函  数: Set_Frame_Rate                                                        *
*功  能: 设置帧频值，自动选择与参数接近的可设置参数值                          *
*参  数: 帧频值，Hz                                                            *
*******************************************************************************/ 
void Set_Frame_Rate(unsigned int frm){
	unsigned char n = 0x00;
	
	if(frm < FRAME_RATE_CODE[0]){//如果小于最低帧频值
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xe7);//默认值：80Hz
	}else{
		if(frm > FRAME_RATE_CODE[31]){//如果帧频值大于最大值
			SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xe7);//恢复默认值
		}else{//在正常值范围内
			for(n=0;n<31;n++){
				if((FRAME_RATE_CODE[n] <= frm) && (frm < FRAME_RATE_CODE[n+1])){
					SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xe0 | n));
				}
			}
		}
	}
}

/*******************************************************************************
*函  数: Set_Bias                                                              *
*功  能: 设置偏压值                                                            *
*参  数: 值                                                                    *
*******************************************************************************/ 
void Set_Bias(unsigned char bs)
{
	switch(bs){
		case 4:// 1/4bias
			SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xd0);
		break;
		case 3:// 1/3bias
			SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xd3);
		break;
		case 2:// 1/2bias
			SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xd2);
		break;
		default:
			
		break;
	}
}

/*******************************************************************************
*函  数: Set_Position                                                          *
*功  能: 设置任一像素坐标值                                                    *
*参  数: Y = 0 对应COM0——COM7                                                *
*        Y = 1 对应COM8                                                        *
*        X 可设置范围 0——101                                                 *
*******************************************************************************/ 
void Set_Position(unsigned char Y,unsigned char X)
{
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0x80 | (( X >> 4) & 0x07) ));//写X的高3位
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0x90 | (X & 0x0F) ));        //写X的低四位
	
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xa0 | (Y & 0x01)));         //写Y地址
}

/*******************************************************************************
*函  数: Draw_Line                                                             *
*功  能: 控制任一像素函数，即向指定Y地址和X地址写一个字节的数据                *
*参  数: Y = 0 ，DB7--DB0都有效                                                *
*        Y = 1 , 只有DB0有效                                                   *
*******************************************************************************/ 
void Draw_Line(unsigned char Y,unsigned char X, unsigned char dat1,unsigned char dat2,unsigned char CNT)
{
	unsigned char n = 0x00;
	
	Set_Position(Y,X);
	LCD_CS = 0;
	SPI_Send_Byte(WRITE_MODE);
	SPI_Send_Byte(WRITE_RAM_BYTE);
	
	for(n=0;n<CNT;n++){
		SPI_Send_Byte(dat1);
		//Delayms();
		n++;
		//if(n<CNT){
		SPI_Send_Byte(dat2);
			//Delayms();
		//}
		
	}
	if((CNT & 0x01) == 0x00){ SPI_Send_Byte(0x00); }
	
	LCD_CS = 1;
}

/*******************************************************************************
*函  数: Clear_LCD_All                                                         *
*功  能: 清除LCD上所有的数据，使之显示空白                                     *
*参  数: 无                                                                    *
*******************************************************************************/ 
void Clear_LCD_ALL(void)
{
	unsigned char y = 0x00;

	for(y=0;y<Y_SIZE;y++){ Draw_Line(y,0x00,0x00,0x00,SEG_CNT); }
	
	for(y=0;y<SEG_CNT;y++){ LCD_RAM[y] = 0x00; }
	
	Current_X_POS = 0x00;
}

/*******************************************************************************
*函  数: Clear_LCD_Dianzhen                                                    *
*功  能: 清除点阵区                                                            *
*参  数: 无                                                                    *
*******************************************************************************/ 
void Clear_LCD_Dianzhen(void)
{
	unsigned char n = 0x00;
		
	Draw_Line(0x00,0x00,0x00,0x00,X_SIZE);
	
	Current_X_POS = 0x00;
	for(n=0;n<X_SIZE;n++){ LCD_RAM[n] = 0x00; }
}

/*******************************************************************************
*函  数: Clear_LCD_NUM_ICON                                                    *
*功  能: 清除数字、图标区                                                      *
*参  数: 无                                                                    *
*******************************************************************************/ 
void Clear_LCD_NUM_ICON(void)
{
	Draw_Line(0x00,NUM_START_ADDR,0x00,0x00,(SEG_CNT - NUM_START_ADDR));
	Draw_Line(0x01,0x00,0x00,0x00,SEG_CNT);
}

/*******************************************************************************
*函  数: LCD_Refresh_Dianzhen                                                  *
*功  能: 刷新点阵区数据                                                        *
*参  数: 无                                                                    *
*******************************************************************************/ 
void LCD_Refresh_Dianzhen(void)
{
	unsigned char n = 0x00;
	
	Set_Position(0x00,0x00);//设置Y=0，X=0
	
	LCD_CS = 0;
	SPI_Send_Byte(WRITE_MODE);
	SPI_Send_Byte(WRITE_RAM_BYTE);
	
	for(n=0;n<X_SIZE;n++){ SPI_Send_Byte(LCD_RAM[X_SIZE - 1 - n]); }
	
	LCD_CS = 1;
}

/*******************************************************************************
*函  数: LCD_Refresh_NUM                                                       *
*功  能: 刷新数字区数据                                                        *
*参  数: 无                                                                    *
*******************************************************************************/ 
void LCD_Refresh_NUM(void)
{
	unsigned char n = 0x00;
	
	Set_Position(0x00,NUM_START_ADDR);//设置Y=0，X=95
	
	LCD_CS = 0;
	SPI_Send_Byte(WRITE_MODE);
	SPI_Send_Byte(WRITE_RAM_BYTE);
	
	for(n=NUM_START_ADDR;n<SEG_CNT;n++){ SPI_Send_Byte(LCD_RAM[n]); }
	SPI_Send_Byte(0x00);
	
	LCD_CS = 1;
	
}

/*******************************************************************************
*函  数: Show_ASCII88                                                          *
*功  能: 在指定位置显示一个8*8的字符                                           *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_ASCII88(unsigned char X,unsigned char dat,unsigned char en_dis)
{//X = 0--12
	unsigned char n = 0x00;
	unsigned int tmp = 0x0000;
	unsigned char chr = 0x00;
	
	dat = dat - 32;
	tmp = dat;
	
	for(n=0;n<7;n++){
		chr = ASCII_ZX_DX[tmp*8+ n];
		LCD_RAM[X*7 + n] = chr;
		Draw_Line(0x00,94-X*7-n,chr,chr,0x01);
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_Dianzhen(); }
}

/*******************************************************************************
*函  数: Show_String_88                                                        *
*功  能: 在指定位置显示一个字符串                                              *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_String_88(unsigned char X,unsigned char* DAT){
	unsigned char n = 0x00;
	
	n = 0x00;
	while(DAT[n] != 0x00){
		if(DAT[n+1] != 0x00){
			Show_ASCII88((X+n),DAT[n],DISABLE);
		}else{
			Show_ASCII88((X+n),DAT[n],ENABLE);
		}
		
		n++;
	}
	
}

/*******************************************************************************
*函  数: Show_Current_Pos_Char                                                 *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_Current_Pos_Char(unsigned char dat)
{
	Show_ASCII88(Current_X_POS,dat,DISABLE);
	Current_X_POS++;
	if(Current_X_POS == 13){ Current_X_POS = 0x00; }
}

/*******************************************************************************
*函  数: LCD_Temperature_Init                                                  *
*功  能: 初始化温度模块是否打开，测试是否过滤，VOP是否补偿                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void LCD_Temperature_Init(unsigned char Measure_en,unsigned char Measure_filter,unsigned char Vop_Buchang)
{
	unsigned char cmd = 0x00;
        //测量使能	
	if(Measure_en){ cmd = cmd | 0x01; }
        //温度滤波使能
	if(Measure_filter){ cmd = cmd | 0x02; }
        //VOP补偿使能
	if(Vop_Buchang){ cmd = cmd | 0x04; }
	
	SPI_Write(WRITE_MODE,Temp_CTRL,cmd);
}

/*******************************************************************************
*函  数: LCD_Init                                                              *
*功  能: 液晶屏初始化                                                          *
*参  数: 无                                                                    *
*******************************************************************************/ 
void LCD_Init(void)
{
	unsigned char n = 0x00;
	
	LCD_RST = 0;
	Delays(5);
	LCD_RST = 1;
	Delays(5);
	
	while(LCD_Init_COMMAND[n] != 0xff){
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,LCD_Init_COMMAND[n]);
		n++;
		Delays(1);
	}
	
	Set_Charge_Pump(ENABLE,3);//3倍压
	Set_VLCD(75);//设置VOP = 6.V// VOP电压范围：3.99V--12V
	Delays(1);
	Set_Inversion_Mode(3);//3-line inversion
	Delays(1);
	Set_Frame_Rate(80);//设置帧频 80Hz
	Delays(1);
	
	LCD_Temperature_Init(ENABLE,DISABLE,DISABLE);//只打开温度测量

	Clear_LCD_ALL();
	
	Set_Display_Mode(DISPLAY_ON);
	Delays(1);
}

#define	ZHENG		0
#define	ONE_NUM		1
#define	FU		2
/*******************************************************************************
*函  数: Show_8NUM                                                             *
*功  能: 显示8段字符                                                           *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_8NUM(unsigned char num,unsigned char en_dis)
{
	switch(num){
		case ZHENG:
		LCD_RAM[100] = LCD_RAM[100] & 0x7f;//8G不显示，显示正值
		LCD_RAM[99]  = LCD_RAM[99] & 0x7f;//8不显示
		break;
		case FU:
		LCD_RAM[100] = (LCD_RAM[100] & 0x7f) | 0x80;//8G显示，显示负值
		break;
		case ONE_NUM:
		LCD_RAM[99]  = (LCD_RAM[99] & 0x7f) | 0x80;//8显示1
		break;
		default:
		LCD_RAM[99]  = LCD_RAM[99] & 0x7f;//8不显示
		LCD_RAM[100] = LCD_RAM[100] & 0x7f;//8不显示
		break;
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_NUM(); }
}

const unsigned char NUM_9_10_CODE[]={
	0x50,0xf0,//0
	0x50,0x00,//1
	0x60,0xb0,//2
	0x70,0x90,//3
	0x70,0x40,//4
	0x30,0xd0,//5
	0x30,0xf0,//6
	0x50,0x80,//7
	0x70,0xf0,//8
	0x70,0xd0,//9
	0x70,0xe0,//A
	0x30,0x70,//b
	0x00,0xf0,//C
	0x70,0x30,//d
	0x20,0xf0,//E
	0x20,0xe0,//F
	
};

/*******************************************************************************
*函  数: Show_9NUM                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_9NUM(unsigned char dat,unsigned char en_dis)
{
	if(dat < 16){
		LCD_RAM[99]  = (LCD_RAM[99] & 0x8f) | NUM_9_10_CODE[dat*2];
		LCD_RAM[101] = (LCD_RAM[101] & 0x0f) | NUM_9_10_CODE[dat*2+1];
		
	}else{
		LCD_RAM[99]  = (LCD_RAM[99] & 0x8f);
		LCD_RAM[101] = (LCD_RAM[101] & 0x0f);
		
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_NUM(); }
}

/*******************************************************************************
*函  数: Show_10NUM                                                            *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_10NUM(unsigned char dat,unsigned char en_dis)
{
	if(dat < 16){
		LCD_RAM[97] = (LCD_RAM[97] & 0x8f) | NUM_9_10_CODE[dat*2];
		LCD_RAM[98] = (LCD_RAM[98] & 0x0f) | NUM_9_10_CODE[dat*2+1];
		
	}else{
		LCD_RAM[97] = (LCD_RAM[97] & 0x8f);
		LCD_RAM[98] = (LCD_RAM[98] & 0x0f);
		
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_NUM(); }
	
}

/*******************************************************************************
*函  数: Show_11NUM                                                            *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
#define	ICON_SHESHIDU		130
void Show_11NUM(unsigned char chn,unsigned char en_dis){
	if(chn == ICON_SHESHIDU){//显示摄氏度℃
		LCD_RAM[96] = (LCD_RAM[96] & 0x3f) | 0x40;
		LCD_RAM[97] = (LCD_RAM[97] | 0x80);
		
	}else{
		LCD_RAM[96] = (LCD_RAM[96] & 0x3f);
		LCD_RAM[97] = (LCD_RAM[97] & 0x7f);
		
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_NUM(); }
}

/*******************************************************************************
*函  数: Read_Temperatue                                                       *
*功  能: 取温度数据                                                            *
*参  数: 无                                                                    *
*******************************************************************************/ 
unsigned char Read_Temperatue(void)
{// T = 0.6275 * [T] - 40
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	LCD_CLK = 0;
	LCD_CS = 0;
	
	SPI_Send_Byte(READ_MODE);//
	SPI_Send_Byte(0x80);//读取温度的命令

	tmp = 0x00;
	for(n=0;n<8;n++){
		LCD_CLK = 0;
		tmp = tmp << 1;
		if(LCD_SDAOUT){ tmp = tmp + 0x01; }
		LCD_CLK = 1;
	}

	LCD_CS = 1;
	
	return tmp;
}

/*******************************************************************************
*函  数: Show_12NUM                                                            *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_12NUM(unsigned char dat,unsigned char en_dis)
{
	switch(dat){
		case 0x00://不显示
				LCD_RAM[95] = LCD_RAM[95] & 0x1f;//不显示
		break;
		case 0x01://显示1
				LCD_RAM[95] = (LCD_RAM[95] & 0x1f) | 0xa0;//显示BC-- 1
		break;
		case 0x02://显示2
				LCD_RAM[95] = (LCD_RAM[95] & 0x1f) | 0xc0;//显示2
		break;
		default:
				LCD_RAM[95] = LCD_RAM[95] & 0x1f;//不显示
		break;
		
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_NUM(); }
}

const unsigned char NUM_13_14_CODE[]={//
	0x0f,0x0a,//0
	0x00,0x0a,//1
	0x0d,0x06,//2
	0x09,0x0e,//3
	0x02,0x0e,//4
	0x0b,0x0c,//5
	0x0f,0x0c,//6
	0x01,0x0a,//7
	0x0f,0x0e,//8
	0x0b,0x0e,//9
	0x07,0x0e,//A
	0x0e,0x0c,//b
	0x0f,0x00,//C
	0x0c,0x0e,//d
	0x0f,0x04,//E
	0x07,0x04,//F
	
};

/*******************************************************************************
*函  数: Show_13NUM                                                            *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_13NUM(unsigned char dat,unsigned char en_dis)
{
	if(dat < 16){
		LCD_RAM[95] = (LCD_RAM[95] & 0xf0) | NUM_13_14_CODE[dat*2];
		LCD_RAM[96] = (LCD_RAM[96] & 0xf1) | NUM_13_14_CODE[dat*2+1];
		
	}else{//不显示
		LCD_RAM[95] = (LCD_RAM[95] & 0xf0);
		LCD_RAM[96] = (LCD_RAM[96] & 0xf1);
		
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_NUM(); }
}

/*******************************************************************************
*函  数: Show_14NUM                                                            *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_14NUM(unsigned char dat,unsigned char en_dis)
{
	if(dat < 16){
		LCD_RAM[97] = (LCD_RAM[97] & 0xf0) | NUM_13_14_CODE[dat*2];
		LCD_RAM[98] = (LCD_RAM[98] & 0xf1) | NUM_13_14_CODE[dat*2+1];
		
	}else{//不显示
		LCD_RAM[97] = (LCD_RAM[97] & 0xf0);
		LCD_RAM[98] = (LCD_RAM[98] & 0xf1);
		
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_NUM(); }
}

/*******************************************************************************
*函  数: Show_15NUM                                                            *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_15NUM(unsigned char dat,unsigned char en_dis)
{
	if(dat < 16){
		LCD_RAM[99]  = (LCD_RAM[99] & 0xf0) | NUM_13_14_CODE[dat*2];
		LCD_RAM[101] = (LCD_RAM[101] & 0xf1) | NUM_13_14_CODE[dat*2+1];
		
	}else{//不显示
		LCD_RAM[99] = (LCD_RAM[99] & 0xf0);
		LCD_RAM[101] = (LCD_RAM[101] & 0xf1);
		
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_NUM(); }
}

/*******************************************************************************
*函  数: Show_Snow                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
#define		ICON_SNOW		129
void Show_Snow(unsigned char chn,unsigned char en_dis)
{
	if(chn == ICON_SNOW){
		LCD_RAM[96] = (LCD_RAM[96] | 0x20);
	}else{
		LCD_RAM[96] = (LCD_RAM[96] & 0xdf);
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}

/*******************************************************************************
*函  数: Show_Maohao                                                           *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
#define		ICON_MAOHAO		128
void Show_Maohao(unsigned char chn,unsigned char en_dis)
{
	if(chn == ICON_MAOHAO){
		LCD_RAM[96] = (LCD_RAM[96] | 0x01);
	}else{
		LCD_RAM[96] = (LCD_RAM[96] & 0xfe);
	}
	
	if(en_dis == ENABLE){ LCD_Refresh_NUM(); }
}

/*******************************************************************************
*函  数: Show_Time                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_Time(unsigned char hour,unsigned char minute,unsigned char second,unsigned char en_dis)
{
	if(en_dis){
		Show_12NUM((hour/10),DISABLE);
		Show_13NUM((hour%10),DISABLE);
		Show_14NUM((minute/10),DISABLE);
		Show_15NUM((minute%10),DISABLE);
		
		Show_Maohao((ICON_MAOHAO + (second & 0x01)),ENABLE);
	}else{
		Show_12NUM((0xff),DISABLE);
		Show_13NUM((0xff),DISABLE);
		Show_14NUM((0xff),DISABLE);
		Show_15NUM((0xff),DISABLE);
		
		Show_Maohao((ICON_MAOHAO + 0x01),ENABLE);
	}
}

#define		ICON_AUTO			0
#define		ICON_WEST			1
#define		ICON_SOUTH			2
#define		ICON_CITY			3
#define		ICON_EAST			13
#define		ICON_MILE			17
#define		ICON_KM_M			18
#define		ICON_KM_K			23
#define		ICON_FRONT_LIGHT		87
#define		ICON_CONFIG			70
const unsigned ICON_CODE[]={
	0,
	1,
	2,
	3,
	13,
	17,
	18,
	23,
	87,
	70,
	
};

/*******************************************************************************
*函  数: Show_Icon                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_Icon(unsigned char chn,unsigned char en_dis)
{
	unsigned char n = 0x00;
	
	for(n=0;n<10;n++){
		if(chn == ICON_CODE[n]){//是指定的图标
			if(en_dis == ENABLE){
				Draw_Line(0x01,chn,0x01,0x01,0x01);//显示相应图标
			}else{
				Draw_Line(0x01,chn,0x00,0x00,0x01);//不显示相应图标
			}
			goto SHOW_ICON_EXIT;
		}
	}
	
	if(chn == ICON_SHESHIDU){//如果是控制℃图标
		Show_11NUM((ICON_SHESHIDU+(1-en_dis)),ENABLE);
	}else{
		if(chn == ICON_SNOW){//如果是控制雪花图标
			Show_Snow((ICON_SNOW+(1-en_dis)),ENABLE);
		}else{
			if(chn == ICON_MAOHAO){//如果是控制时钟冒号图标
				Show_Maohao((ICON_MAOHAO+(1-en_dis)),ENABLE);
			}
		}
	}
	
SHOW_ICON_EXIT:
	n = 0x00;
	
}

/*******************************************************************************
*函  数: Show_Temperatue                                                       *
*功  能: 显示温度                                                              *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_Temperatue(void)
{
	unsigned char tmp = 0x00;
	unsigned long dat = 0x00000000;
	unsigned int TT = 0x0000;
	unsigned char bai = 0x00;
	unsigned char shi = 0x00;
	unsigned char ge = 0x00;
	
	tmp = Read_Temperatue();
	
	if(tmp != 0xff){//读出来的数据不能是0xff
		if(tmp > 63){//正温度值
			dat = tmp;
			dat = dat * 6275;
			dat = dat - (400000);//温度值放大10000倍
			dat = dat + 5000;// 温度值四舍五入
			dat = dat / 10000;// 温度值恢复正常
			
			TT = (unsigned int)(dat);//取整，显示温度区不能显示小数
			
			bai = TT / 100;//温度百位值
			shi = (TT - bai*100)/10;//温度十位值
			ge = TT % 10;//温度个位值
			
			
			Show_8NUM(bai,DISABLE);
			Show_9NUM(shi,DISABLE);
			Show_10NUM(ge,DISABLE);
			
		}else{
			dat = tmp;
			dat = dat * 6275;
			dat = (400000) - dat; // 负温度值
			dat = dat - 5000;//温度值 四舍五入
			dat = dat / 10000;// 温度回复正常
			
			TT = (unsigned int)(dat);
			
			shi = TT / 10;//温度十位值
			ge = TT % 10;//温度个位值
			
			Show_8NUM(FU,DISABLE);
			Show_9NUM(shi,DISABLE);
			Show_10NUM(ge,DISABLE);
			Show_Snow(ICON_SNOW,DISABLE);
		}
		Show_11NUM(ICON_SHESHIDU,ENABLE);
	}
}

const unsigned char NUM_1_CODE[]={//D, C, B, A, F, G, E
	0xfa,//0
	0x60,//1
	0xb6,//2
	0xf4,//3
	0x6c,//4
	0xdc,//5
	0xde,//6
	0x70,//7
	0xfe,//8
	0xfc,//9
	0x7e,//A
	0xce,//b
	0x9a,//C
	0xe6,//d
	0x9e,//E
	0x1e,//F
};

/*******************************************************************************
*函  数: Show_1NUM                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_1NUM(unsigned char dat)
{
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_1_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(88+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(88+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(88+n),0x00,0x00,0x01);
		}
	}
}

const unsigned char NUM_2_3_CODE[]={// C, B, A, F, G, E, D 
	0xf6,//0
	0xc0,//1
	0x6e,//2
	0xea,//3
	0xd8,//4
	0xba,//5
	0xbe,//6
	0xe0,//7
	0xfe,//8
	0xfa,//9
	0xfc,//A
	0x9e,//b
	0x36,//C
	0xce,//d
	0x3e,//E
	0x3c,//F
	
};

/*******************************************************************************
*函  数: Show_2NUM                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_2NUM(unsigned char dat)
{
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(59+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(59+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(59+n),0x00,0x00,0x01);
		}
	}
	
}

/*******************************************************************************
*函  数: Show_3NUM                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_3NUM(unsigned char dat)
{
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(52+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(52+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(52+n),0x00,0x00,0x01);
		}
	}
}

/*******************************************************************************
*函  数: Show_4NUM                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_4NUM(unsigned char dat)
{
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(45+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(45+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(45+n),0x00,0x00,0x01);
		}
	}
}

/*******************************************************************************
*函  数: Show_5NUM                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_5NUM(unsigned char dat)
{
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(38+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(38+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(38+n),0x00,0x00,0x01);
		}
	}
}

/*******************************************************************************
*函  数: Show_6NUM                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_6NUM(unsigned char dat)
{
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(31+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(31+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(31+n),0x00,0x00,0x01);
		}
	}
	
}

/*******************************************************************************
*函  数: Show_7NUM                                                             *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_7NUM(unsigned char dat)
{
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(24+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(24+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(24+n),0x00,0x00,0x01);
		}
	}
}

const unsigned int MI_CODE[]={
	0x0955,//0
	0x0005,//1
	0x0996,//2
	0x0897,//3
	0x00c7,//4
	0x08d3,//5
	0x09d3,//6
	0x0015,//7
	0x09d7,//8
	0x08d7,//9
	0x01d7,//A
	0x09c3,//b
	0x0950,//C
	0x0987,//d
	0x09d2,//E
	0x01d2,//F
	
};

/*******************************************************************************
*函  数: Show_Mi                                                               *
*功  能: 设置打开/关闭显示                                                     *
*参  数: 模式值                                                                *
*******************************************************************************/ 
void Show_Mi(unsigned char dat)
{
	unsigned char n = 0x00;
	unsigned int tmp = 0x00;
	
	if(dat < 16){
		tmp = MI_CODE[dat];
		for(n=0;n<13;n++){
			if(n != 9){//空一个地址，此地址为“E”
				if((tmp & 0x0001) == 0x0001){
					Draw_Line(0x01,(4+n),0x01,0x01,0x01);
				}else{
					Draw_Line(0x01,(4+n),0x00,0x00,0x01);
				}
			}
			tmp = tmp >> 1;
		}
	}else{
		for(n=0;n<13;n++){
			if(n != 9){//空一个地址，此地址为“E”
				Draw_Line(0x01,(4+n),0x00,0x00,0x01);
			}
		}
	}
}

/*******************************************************************************
*函  数: Show_Multi_NUM                                                        *
*功  能: 将所有数字统一管理                                                    *
*参  数: 模式值                                                                *
*******************************************************************************/ 
//
void Show_Multi_NUM(unsigned char chn,unsigned char num){
	
	switch(chn){
		case 0x01://第1个7段数字
					Show_1NUM(num);
		break;
		case 0x02://第2个7段数字
					Show_2NUM(num);
		break;
		case 0x03://第3个7段数字
					Show_3NUM(num);
		break;
		case 0x04://第4个7段数字
					Show_4NUM(num);
		break;
		case 0x05://第5个7段数字
					Show_5NUM(num);
		break;
		case 0x06://第6个7段数字
					Show_6NUM(num);
		break;
		case 0x07://第7个7段数字
					Show_7NUM(num);
		break;
		case 0x08://第8个7段数字
					Show_8NUM(num,ENABLE);
		break;
		case 0x09://第9个7段数字
					Show_9NUM(num,ENABLE);
		break;
		case 10://第10个7段数字
					Show_10NUM(num,ENABLE);
		break;
		case 11://第11个7段数字
					Show_11NUM(num,ENABLE);
		break;
		case 12://第12个7段数字
					Show_12NUM(num,ENABLE);
		break;
		case 13://第13个7段数字
					Show_13NUM(num,ENABLE);
		break;
		case 14://第14个7段数字
					Show_14NUM(num,ENABLE);
		break;
		case 15://第15个7段数字
					Show_15NUM(num,ENABLE);
		break;
		case 16://显示米 字符
				Show_Mi(num);
		break;
		
		default:
		break;
	}
}


void Test_LCD(void){
	Show_String_88(0,(unsigned char *)"   Dushanwu  ");
	Delays(DELAY_VALUE);
}
