 
#ifndef __PTASK_H
#define __PTASK_H

#include "parameter.h"
//#define MAXTASKS 8
 
#define lc_type  unsigned char
#define SEM unsigned int 

//volatile unsigned char timers[MAXTASKS];

/******************************************************
Micro    : PTH_BEGIN
Note     : 启动任务处理，放在函数开始处
Updata   : 2012-12-14
*******************************************************/
#define PTH_BEGIN                                      \
    static lc_type _lc; switch(_lc){default: 

   
/******************************************************
Micro    : PTH_END
Note     : 结束任务,放在函数的最后
Updata   : 2012-12-07
*******************************************************/
#define PTH_END                 ;};  _lc=0; return 255;



//延时等待的tickets 最大为250
#define WaitX(tickets) do {_lc=__LINE__+((__LINE__%256)==0); \
       	return tickets;} while(0); case __LINE__+((__LINE__%256)==0): 

//定时器的赋值，并非你想象那样多余，原有定时器部分的赋值操作是必须的，否则，任务调度会出问题：非循环运行的任务会变为循环运行。
#define RunTask(TaskName,TaskID) do { if (timers[TaskID]==0) \
   timers[TaskID]=TaskName();}  while(0);
//前面的任务优先保证执行 
#define RunTaskA(TaskName,TaskID) { if (timers[TaskID]==0) \
	{timers[TaskID]=TaskName(); continue;} }  

#define CallSub(SubTaskName) do {lc_type currdt; WaitX(0); \
   currdt=SubTaskName(); if(currdt!=255) return currdt;} while(0);
 

#define UpdateTimers() {unsigned char i; for(i=MAXTASKS;i>0;i--) \
 {if((timers[i-1]!=0)&&(timers[i-1]!=255)) timers[i-1]--;}}

//初始化信号量
#define InitSem(sem) sem=0;
//等待信号量
#define WaitSem(sem) do{ sem=1; WaitX(0); if (sem>0) return 1;} while(0);
//等待信号量或定时器溢出， 定时器tickets 最大为0xFFFE
#define WaitSemX(sem,tickets)  do { sem=tickets+1; WaitX(0);  \
  if(sem>1){ sem--; return 1;} } while(0);
//发送信号量
#define SendSem(sem)  do {sem=0;} while(0);


#endif
