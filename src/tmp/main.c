#include <STC89C51.h>

/**************小小调度器开始啦 *********************************/
#define MAXTASKS 3	    //顶层任务数量，可按需增加，最大为255个。
char  timers[MAXTASKS];
char currid; //当前运行的任务号
#define _SS   static char lc=0; switch(lc){   case 0: 	 //跳转开始
#define _EE   }; lc=0;  //跳转结束
#define DelayX(b)  settimer(&lc,__LINE__,b); return ; case __LINE__: //任务内延时等待“函数”
#define RunTask(a,b) currid=b; if (timers[b]==0){timers[b]=-1; a();} //运行顶层任务
#define CallSub(x)  DelayX(0);x(); if (timers[currid]!=-1) return;  	 //调用子任务
void settimer(char *lc,char  line,char d){//设置定时器
  *lc=line;  timers[currid]=d;
}
/****************小小调度器结束啦*********************************/

sbit KEY = P3^2;
unsigned char code numtab[16]={0x24,0x6F,0xE0,0x62,0x2B,0x32,0x30,0x67,0x20,0x22,0x21,0x38,0xB4,0x68,0xB0,0xB1};

sfr IAP_CONTR = 0xC7; 
sfr WDT_CONTR = 0xC1;
//清除看门狗
void clr_wdt()
{
  WDT_CONTR =0x3C; 
}
//初始化定时器，产生10ms中断
void InitT0()
{
	TMOD = 0x21;
	IE |= 0x82;  // 12t
	TL0=0Xff;
	TH0=0Xb7;
	TR0 = 1;
}
//定时器中断处理函数
void INTT0(void) interrupt 1 using 1
{
    unsigned char i;
	TL0=0Xff;    //10ms 重装
	TH0=0Xb7;
	//逻辑定时器处理
    for (i=0;i<MAXTASKS;i++){
     if (timers[i]>0) {
	   timers[i]--;
	 }
    }	
}

sbit LED1= P2^4;  

//任务一，状态机写法
void ontimer0(){
  LED1=!LED1;	// LED1引脚接在发光管负极，LED1=0 为亮，LED1=1为灭  
  if (LED1) timers[currid]=45;	//450mS 灭
  else timers[currid]=5;	//50ms  亮
}

//任务二，状态机写法
char keycount=0;
void task1(){
 if(KEY==0) {
   keycount++;
   if (keycount>20) IAP_CONTR = 0x60;
 }
 else{
    keycount=0;
 }
 timers[currid]=5;	 //重装定时器
}

//这是一个子任务函数
void  subtask()
{
 static char i;
_SS
 for(i=0;i<=5;i++){
  DelayX(20);
  P1=numtab[i];
 }
_EE
}

//任务三，“线程”写法，复杂任务请用这种方式来写，比传统状态机写法更简单，更自然，而且更省ROM！
void  task2()
{
 static char i;	 //变量请申明为静态的。
_SS

while(1){
 for(i=0;i<=9;i++){	  //先从0--9快速显示，间隔200mS
  DelayX(20);
  P1=numtab[i];
 }
 for(i=0;i<=9;i++){//然后从0--9慢速显示，间隔500mS
  DelayX(50);
  P1=numtab[i];
  CallSub(subtask); //顶层任务的任何地方都可以调用子任务
 }
  //CallSub(subtask); //顶层任务的任何地方都可以调用子任务
}

_EE
}

void main()
{
	P3M0 = 0x00;
	P3M1 =0x00;
	P1 = 0xff;         //关LED数码管显示

	InitT0();	  //初始化定时器，以便产生10ms中断
	KEY =1;		  //按键IO口	
    
	while(1){
  	   clr_wdt(); //清除看门狗

	   //这里，只需运行顶层任务即可
	   RunTask(ontimer0,0);	//ontimer0任务放在第0个任务槽
	   RunTask(task1,1);	//task1任务放在第1个任务槽
	   RunTask(task2,2);    //task2任务放在第2个任务槽
	   //...还可以增加更多任务
  	}
}
