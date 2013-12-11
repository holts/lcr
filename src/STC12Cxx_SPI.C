/******************************************************************************
 * STC12C5A60S2 SPI驱动
******************************************************************************/

#include "config_cog.h"

/*
void InitSPI()
{
    P1M0 &= 0xbf; 
    P1M1 |= 0x40; 

    SPDAT = 0;                  //initial SPI data
    SPSTAT = SPIF | WCOL;       //clear SPI status
    SPCTL = SSIG | SPEN | MSTR | CPHA | CPOL | SPSPEEDLL;        //master mode      | DORD 
}

U8 SPI_Send(U8 dat)
{
    U8 tmp;
    EA = 0;
//    SPISS = 0;                  //pull low slave SS
    SPSTAT = SPIF | WCOL;       //clear SPI status
    SPDAT = dat;                //trigger SPI send
    while (!(SPSTAT & SPIF));   //wait send complete
    tmp = SPDAT;
//    SPISS = 1;                  //push high slave SS
    EA = 1;
    return tmp;               //return received SPI data
}
*/

//void SPI_HI_Speed(U8 h)
//{
//    h = h;
   /*
    if(h)
    {
        SPCTL = SSIG | SPEN | MSTR | SPSPEEDHH;        //master mode      | DORD    CPHA | CPOL | 
    }else{
        SPCTL = SSIG | SPEN | MSTR | SPSPEEDLL;        //master mode      | DORD    CPHA | CPOL | 
    }
    */
//}
 


/* 模拟方式---------------------------------- */
uchar SPI_Sends(uchar out_data)
 {
 	uchar i,n;
	for(i=0;i<8;i++)
	{
		n<<=1;
		SCK=0;
		MOSI=(out_data&0x80);
		if(MISO)	n++;
		SCK=1;
		out_data<<=1;
	}
	return n;  
 }


