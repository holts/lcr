
#define uchar unsigned char
#define uint  unsigned int
#define ulong  unsigned long
#include <reg52.h>
#include <math.h>

//==========================================================================
sbit lcd_RS = P0^6; //�����������λ,0����1����
sbit lcd_RW = P0^5; //��дλ,0д1��
sbit lcd_EN = P0^4; //ʹ��λ,�½��ش���
sbit lcd_D4 = P0^3; //���ݶ˿�D4
sbit lcd_D5 = P0^2; //���ݶ˿�D5
sbit lcd_D6 = P0^1; //���ݶ˿�D6
sbit lcd_D7 = P0^0; //���ݶ˿�D7
//==========================================================================
code char path[40]={
0x00,0x00,0x0e,0x11,0x11,0x0a,0x1b,0x00,
0x00,0x11,0x11,0x13,0x1d,0x10,0x10,0x10,
0x0e,0x11,0x1f,0x11,0x11,0x0e,0x00,0x00,
0x1C,0x10,0x1B,0x12,0x1B,0x01,0x07,0x00,
0x04,0x0e,0x1f,0x04,0x1f,0x0e,0x04,0x00
};//�Զ������
code char *lcr[7]   = {"2000", " 200", "20.0","2.00", " 20", " 20","2.0"};
code char *feqc[3]  = {"100Hz"," 1KHz" ,"7.8KH"};
code char *zero[2]  = {"Open zero......","Short zero...OK."};
char binLian=0,ATA=0,ATB=0,ATD=0;
char OX[8]={0,0,0,0,0,0,8,0};
int c3 = 0,c4 = 0; 
xdata char isQ=1,REL=0;
xdata uchar menu=1,menu2=0; //�˵�����
xdata float TR,TX; //��������
xdata float RELDAT; //��Բ�������


void lcd_delay(int n){ //LCDר����ʱ����
  //32MHz��Ƶ�£�Լѭ��3000���ӳ�1����
  int i,j;
  if(n<0)    { for(i=0;i< 30;i++); return; } //10us
  if(n== 0)  { for(i=0;i<150;i++); return; } //50us
  for(;n;n--){ for(j=0;j<3000;j++);        } //n����
}
//==========================================================================
void lcd_B(char f, uchar c, char t){ //��������ʽ�ӿ�LCD��7����
  //f=0д������, f=1дRAM����, f=2����ַ�����æ��, f=3��RAM����
  lcd_EN = 0;
  lcd_RS = f%2;
  lcd_RW = f/2%2;
  //�������λ
  lcd_D4 = c & 16;
  lcd_D5 = c & 32;
  lcd_D6 = c & 64;
  lcd_D7 = c & 128;
  lcd_EN = 1;  lcd_delay(-1);  lcd_EN = 0; //ʹ������
  if(f==4) { lcd_delay(t); return; }
  //�������λ
  lcd_D4 = c & 1;
  lcd_D5 = c & 2;
  lcd_D6 = c & 4;
  lcd_D7 = c & 8;
  lcd_EN = 1;  lcd_delay(-1);  lcd_EN = 0; //ʹ������
  lcd_delay(t);  //��ͬ������,��Ӧʱ�䲻ͬ,����������Ҫ2ms
}
//==========================================================================
void lcd_init(){ //LCD1602 ��ʼ��
  //��������ģʽ������9�����裬��ʼ�������ʱ�ϳ���Լ65ms��ʱ�޲��ɼ�
  lcd_delay(20); //����lcd֮ǰ����ʱ����15ms��ֱ��VDD����4.5V
  lcd_B(4, 0x30, 9); //��8��ģʽ,����ʱ����4.1ms
  lcd_B(4, 0x30, 5); //��8��ģʽ,����ʱ����100us
  lcd_B(4, 0x30, 5); //��8��ģʽ,�ֲ���δָ����ʱ
  lcd_B(4, 0x20, 5); //��������ģʽ
  lcd_B(0, 0x28, 5); //����ģʽ˫����ʾ
  lcd_B(0, 0x0C, 5); //����ʾ��
  lcd_B(0, 0x80, 5); //RAMָ�붨λ
  lcd_B(0, 0x01, 5); //������������ʼ��LCD
}
//=========================�������ܳ��ú���=================================
void lcd_cls()         { lcd_B(0, 0x01+0, 2);  } //����
void lcd_cur0()        { lcd_B(0, 0x0C+0, 0);  } //���ع��
void lcd_goto1(uchar x){ lcd_B(0, 0x80+x, 0);  } //����DDRAM��ַ,��1��xλ
void lcd_goto2(uchar x){ lcd_B(0, 0xC0+x, 0);  } //����DDRAM��ַ,��2��xλ
void lcd_putc(uchar d) { lcd_B(1, 0x00+d, 0);  } //�ַ����
void lcd_puts(uchar *s){ for(; *s; s++) lcd_B(1,*s,0); } //�ִ����
//==============�ַ���ʾ����====================

void lcd_putp(float a,float b,char bo,char n, float qmin){ //����λ��ʾ����,n�ǵ�λ����,qmin����СλȨֵ(�����޶���Ч����)
  code uchar resistorB[] = {'p','n',1,'m',0,'k','M','G'}; //��λ��
  char i,j, c=0,digW=4, h=3, fh[2]={' ','+'};
  long d,q,qm,Q=1; //���λȨ
  float f,g=1,k=1;
  if(bo==2){k=2;}
  if(a<0) fh[0] = '-', a = -a;
  if(b<0) fh[1] = '-', b = -b;
  if(a>b) f = a; else f = b;
  for(i=1;i<4;i++) Q *= 10;
  for(i=0;i<3;i++){ if(f*g >= k*1000) g/=1000, c++; } //��3λΪ��λ�ƶ�С����,����
  for(i=0;i<n;i++){ if(f*g < k)     g*=1000, c--; }   //��3λΪ��λ�ƶ�С����,����
  if ((n==1)&&(c==-1)){
  digW=2;                 //�ƶ�1λС����
  }else if ((n==2)&&(c==-2)){
  if(frqB[Frq_idx]!=7813) {digW=2;   //�ƶ�1λС����
  }else{ digW=3; }         //�ƶ�2λС����
  }else if ((n==4)&&(c==-4)){
  if(frqB[Frq_idx]!=7813) {digW=2;   //�ƶ�1λС����
  }else{ digW=3; }         //�ƶ�2λС����
  }
  for(i=1;i<digW && f*g<k*Q;i++) g*=10,h--;           //�����ƶ�С���㣬ʹ֮����
  qm = g*qmin;
  for(i=0;i<2;i++){
   if(i) d = b*g+0.5;      //ȡ��ʵ��
   else  d = a*g+0.5;      //ȡ���鲿
   if(qm) d+=qm/2, d-=d%qm;//ȥ��С��qmin��β��
   if(d>=Q*20) d=(d+0.5)/10, h++;
   q = Q;
   if(d>=Q*10){
     d -= Q*10;
     if(fh[i]=='-') {lcd_puts("-1");
     }else          {lcd_puts(" 1");}
   }else{
   lcd_putc(fh[i]); if(bo==2)lcd_putc('0');      //��ʾ����
   }
   for(j=0; j<4; j++){  //�������
    lcd_putc(d/q+48);      //����
    if(j==h) lcd_putc('.');//С����
    d %= q, q /= 10;  
   }
   if(!bo||bo==2) break;     //����ʾ�鲿
  }
  if((n!=1)&&(resistorB[c+4]==0))lcd_putc(' '); else lcd_putc(resistorB[c+4]);//��λ
  if((n==1)&&(resistorB[c+4]==0)) OX[6]=32; else OX[6]=8; //�Զ���λ
}

void lcd_putf(float a, char n, float qmin) //����λ��ʾ������,n�ǵ�λ����
  { lcd_putp(a,0,0,n,qmin); }
void lcd_int(int a,char w){ //������ʾ������
  char i=0, s[5] = {' ',' ',' ',' ',' '};
  if(a<0) { a=-a; lcd_puts("-"); }
  else    lcd_puts(" ");
  do{
   s[i++] = a%10+48;
   a /= 10;
  }while(a);
  for(;w;w--) lcd_putc(s[w-1]);
}

//===============================��ʱ����===================================
void delay(uint loop) { uint i; for(i=0;i<loop;i++); } //��ʱ����
void delay2(uint k)   { for(;k>0;k--) delay(10000);  } //����ʱ,k=100��Լ��Ӧ1��


//=================================ADת��===================================

uint getAD2(){
 ADC_CONTR |= 0x08;             //00001000,��ADC_START=1����A/D ת��
 while ( !(ADC_CONTR & 0x10) ); //�ȴ�A/Dת������(ADC_FLAG==0)
 ADC_CONTR &= 0xE7;             //11100111,��ADC_FLAG=0����������, ��ADC_START=0�ر�A/D ת��
 return ADC_res*4 + ADC_resl;
}

//==================================EEPROWƫ��==============================
/********************
д�ֽ�ʱ�����Խ�ԭ�������е�1��Ϊ0���޷���0��Ϊ1��ֻ��ʹ�ò������0��Ϊ1
Ӧע�⣬��������Ὣ������������
*********************/
int eepEn = 0;
void saEEP(){ //������EEP����
 if(eepEn==12345) IAP_trig = 0x5A;  //����5A
 if(eepEn==12345) IAP_trig = 0xA5;  //����5A����A5��������
 IAP_cmd = 0;      //�ر������
 IAP_contr = 0;    //��EEPROM������
 IAP_trig = 0;
 IAP_addrL = 255; //���ö�ȡ��ַ�ĵ��ֽڣ���ַ�ı����Ҫ����
 IAP_addrH = 255; //���ö�ȡ��ַ�ĸ��ֽڣ���ַ�ı����Ҫ����
}
uchar readEEP(uint k){ //��ȡ
 IAP_addrL = k;    //���ö�ȡ��ַ�ĵ��ֽڣ���ַ�ı����Ҫ����
 IAP_addrH = k>>8; //���ö�ȡ��ַ�ĸ��ֽڣ���ַ�ı����Ҫ����
 IAP_contr = 0x81; //���õȴ�ʱ�䣬1MHz����ȡ7��2M����ȡ6��3Mȡ5��6Mȡ4��12Mȡ3��20Mȡ2��24Mȡ1��30Mȡ0��ǰ��1��ʾ����IAP
 IAP_cmd = 1;      //��ȡֵ1��дȡ2������ȡ3������ʱ�������ֽ���������ߥ��
 saEEP(); //����������
 return IAP_data;
}
void writeEEP(uint k, uchar da){ //д��
 IAP_data = da;    //��������
 IAP_addrL = k;    //���ö�ȡ��ַ�ĵ��ֽڣ���ַ�ı����Ҫ����
 IAP_addrH = k>>8; //���ö�ȡ��ַ�ĸ��ֽڣ���ַ�ı����Ҫ����
 IAP_contr = 0x81; //���õȴ�ʱ�䣬1MHz����ȡ7��2M����ȡ6��3Mȡ5��6Mȡ4��12Mȡ3��20Mȡ2��24Mȡ1��30Mȡ0��ǰ��1��ʾ����IAP
 IAP_cmd = 2;      //��ȡֵ1��дȡ2������ȡ3������ʱ�������ֽ���������ߥ��
 saEEP(); //����������
}
void eraseEEP(uint k){ //����
 IAP_addrL = k;    //���ö�ȡ��ַ�ĵ��ֽڣ���ַ�ı����Ҫ����
 IAP_addrH = k>>8; //���ö�ȡ��ַ�ĸ��ֽڣ���ַ�ı����Ҫ����
 IAP_contr = 0x81; //���õȴ�ʱ�䣬1MHz����ȡ7��2M����ȡ6��3Mȡ5��6Mȡ4��12Mȡ3��20Mȡ2��24Mȡ1��30Mȡ0��ǰ��1��ʾ����IAP
 IAP_cmd = 3;      //��ȡֵ1��дȡ2������ȡ3������ʱ�������ֽ���������ߥ��
 saEEP(); //����������
}



void cs_RW(char rw){
 uchar i,*p = &cs;
 const int offs=512;
 if(rw){
  delay2(20); //�ȴ�(��ֹ������ʶ����̶����ñ�����)
  eraseEEP(offs);
  for(i=0;i<sizeof(cs);i++) writeEEP(i+offs,p[i]);
 }else{
  for(i=0;i<sizeof(cs);i++) p[i]=readEEP(i+offs);
 }
}

//==========================================================================
//==================================LCR������===============================

//==============��Ƶ�ź�DDS��ز���====================

//��λ���ƺ���
void set902() { set90(xw+1); } //��λ����
//==============���̿��ƺ���====================
void setRng2(){ setRng( (Rang_idx+1)%4); } //���̲���
//==============������ƺ���====================
void setGain2(){ setGain((Gain_idx+1)%4); }

//==============AD�����Ը����������=================
uint getAD10(){ //250�β���,��reentrant�������еĵ�Ƭ���޷�����
 uchar i;
 long c;
 for(i=0;i<250;i++) c += getAD2();
 return (c+12)/25;
}
uint getAD10b(){ //250�β���,��reentrant�������еĵ�Ƭ���޷�����
 xdata uint c = 0, c2 = 0;
 uchar i;
 for(i=0;i<250;i++){
  ADC_CONTR |= 0x08;             //00001000,��ADC_START=1����A/D ת��
  while ( !(ADC_CONTR & 0x10) ); //�ȴ�A/Dת������(ADC_FLAG==0)
  ADC_CONTR &= 0xE7;             //11100111,��ADC_FLAG=0����������, ��ADC_START=0�ر�A/D ת��
  c  += ADC_resl;
  c2 += ADC_res;
 }
 c3 = c;
 c = ( c2*4L + c )/25;
 c3= ( c2*4L + c3 ) -c*25;
 return c;
}



//==============LCR����====================
xdata char Sxw[4]={0,1,0,1};    //������ȷ��λ
xdata int Vz[24];               //LCR���������
xdata char pau=0; //��ͣ�����Զ���ת


#define Vfull 9600
#define gad (9600/30)

//==============����Ƶ��====================

int absMax(int a,int b){ //ȡ����������ֵ�����
  if(a<0) a = -a;
  if(b<0) b = -b;
  if(b>a) a = b;
  return a;
}

char yc1=0,yc2=0; //�����ʶ
char slw = 1; //���ٱ���
char chg=0;   //�����л����

void timerInter1(void) interrupt 3 {//T1�ж�,LCR���ݲɼ�
 code int y[5]={0,-15,30,24,0}; //������30
 code int x[5]={4900,5090,5140,5500,6120};
  char g,gb,Rb,i; int c=0,cc=0;
  tims++;
  if(tims>=mT/slw) tims = 0, tim++, c = 1;
  if(tim>=4) tim=0;
  if(pau) return;
  if(!c) return; //timδ��λ����
  c = getAD10b();   //��ȡ��ѹֵ
  c4= c;
  cc= c3;
  c -= cs.zo[Frq_idx];
if(cs.ak!=0){
 for(i=0;i<4;i++){ //�����Ը���
  if(c<x[i]||c>=x[i+1]) continue;
  c += cs.ak*( y[i] + (y[i+1]-y[i]) * ((int)(c-x[i])) / (x[i+1]-x[i]) ) / 30;
  break;
 } 
}
  
  Vxy[tim] = xw<2 ? c : -c;  //���浱ǰ��ѹ
  Vxy[tim+6] = xw<2 ? cc : -cc;  //���浱ǰ��ѹ
  Vxy[tim/2+4] = Gain_idx;    //���浱ǰ����
  Sxw[tim] = ( Sxw[tim]+(c<0 ? 2 : 0) )%4;  //��λ��ת(Ԥ���´ε���λ����ֵ)
  if(tim==1||tim==3){ //���±��л�
    if(tim==1) { K3=1, c = absMax(Vxy[2],Vxy[3]), g=Vxy[5]; yc2 = c>Vfull ? 1:0; }//�л����±�
    if(tim==3) { K3=0, c = absMax(Vxy[0],Vxy[1]), g=Vxy[4]; yc1 = c>Vfull ? 1:0; }//�л����ϱ�
    gb=g, Rb=Rang_idx;
    if(c>Vfull){ if(g==0&&Rang_idx>0&&K3&&isQ) Rb=Rang_idx-1; gb=0; }
    else if(c<gad*1 ) gb = g+3; //����27��
    else if(c<gad*3 ){ gb = g+2;if(c<gad*2&&Rang_idx>=1&&Rang_idx<=2&&K3&&isQ ) { gb=0; Rb=Rang_idx+1; }} //����9��
    else if(c<gad*9)  gb = g+1; //����3��
	if(gb>3) gb = 3;
    if(g==3&&Rang_idx<3&&K3&&isQ) { gb=0; Rb=Rang_idx+1; }
    if(g==0&&c<gad*1&&Rang_idx<3&&K3&&isQ) { gb=0; Rb=Rang_idx+1; }

	setRng(Rb); setGain(gb); //������
    if(gb!=g || Rb!=Rang_idx) slw = 2, chg++; //�������ڸı䣬����ٲ���
    if(tim==3){ if(!chg) slw=1; chg=0; }
  }
  set90( Sxw[ (tim+1)%4 ] ); //��λ��ת
  if(tim==3){ for(i=0;i<12;i++) Vz[i+12]=Vz[i], Vz[i]=Vxy[i]; }
}


void showR(char binLian, char showk){ //��ʾLCR
  xdata float a=0,b=0,c=0,qq=0,dd=0,e,w,L,C;
  xdata int gr=cs.R[Rang_idx], g1=cs.g1, g2=cs.g2;
  xdata int g12 = g1 + g2;          //������󲹳�
  xdata int j12 = (int)cs.j1+cs.j2; //��λ��󲹳�
  xdata float RELOUT;
  xdata long RELTMP;
  xdata float JD = 0, G = 0, cJD;   //��������
  xdata float v[4];
  xdata char ATC=0,ATE=0,d2=0,d3=1;
  xdata float LCR[3];
  //LCR����
  if(Frq_idx<0||Frq_idx>2) return;
  if(frqB[Frq_idx]==7813&&Rang_idx==3) gr += cs.R4b; //7.8kHzʱ�±�������
  //�ɿ����浥Ԫ��������������λ������
  if(frqB[Frq_idx]==7813) g2 += cs.G2b;  //7.8kHzʱ9����������
  if(Vz[4] == 1) JD += cs.j1,  G += g1;
  if(Vz[4] == 2) JD += cs.j2,  G += g2;
  if(Vz[4] == 3) JD += j12,    G += g12;
  if(Vz[5] == 1) JD -= cs.j1,  G -= g1;
  if(Vz[5] == 2) JD -= cs.j2,  G -= g2;
  if(Vz[5] == 3) JD -= j12,    G -= g12;
  JD = (JD - cs.J[Rang_idx]) * Actual_Frq/7813/2000;
  if(frqB[Frq_idx]==977) JD -= cs.phx/2000.0;
  cJD = 1 - JD*JD/2;
  v[0] = Vz[0]+Vz[12]+(Vz[6]+Vz[18])/25.0;
  v[1] = Vz[1]+Vz[13]+(Vz[7]+Vz[19])/25.0;
  v[2] = Vz[2]+Vz[14]+(Vz[8]+Vz[20])/25.0;
  v[3] = Vz[3]+Vz[15]+(Vz[9]+Vz[21])/25.0;
  a = (+( 1.0*v[2]*v[2] + 1.0*v[3]*v[3] )*((gainB[Vz[4]] / gainB[Vz[5]]) /(resistorR[Rang_idx]*(1+gr/10000.0))))+(+( 1.0*v[2]*v[2] + 1.0*v[3]*v[3] )*((gainB[Vz[4]] / gainB[Vz[5]]) /(resistorR[Rang_idx]*(1+gr/10000.0))))*G/10000;  //���油��
  b = (-( 1.0*v[0]*v[2] + 1.0*v[1]*v[3] ))*cJD - (-( 1.0*v[2]*v[1] - 1.0*v[0]*v[3] ))*JD; //��λ����
  c = (-( 1.0*v[0]*v[2] + 1.0*v[1]*v[3] ))*JD + (-( 1.0*v[2]*v[1] - 1.0*v[0]*v[3] ))*cJD; //��λ����  
 if(showk){
  w = 2*3.1415926*Actual_Frq;
  if((showk==5)||(showk==8)||(showk==10)||(showk==13)||(showk==15)||(showk==18)||(showk==19)){d3=0;}

  if((showk==1)||(showk==2)||(showk==3)){
  showk=0;
  KFB = 0;
  lcd_puts("Z=");
  lcd_putf(c4-cs.zo[Frq_idx],0,0);
  return;
}
  if(binLian){ //����
    e = (b*b+c*c)/a;
  if(d3){
    lcd_puts("R=");
	lcd_putp(e/b, 0,2,1, 1e-4); //��ʾ�������迹,��ʾ����ŷ
  }else{
  if((showk==10)||(showk==13)||(showk==15)||(showk==18)){
	C = -c/e/w;
	L = +e/c/w;
    if(C>-2e-12) { lcd_putp(C, 0,2,4, 1e-14); lcd_putc('F'); } //��ʾ����Cֵ,��ʾ��pF
    else         { lcd_putp(L, 0,2,2, 5e-9 ); lcd_putc('H'); } //��ʾ����Lֵ,��ʾ��uH
  }else{
    lcd_putc(10);lcd_puts("=");
    lcd_putf(atan2(c,b)*180/3.1415926,0,0); //��ʾ�Ƕ�
    lcd_puts("    ");
 }

}
  }else{ //����
    e = a;
  if(d3){
    lcd_puts("R=");
	lcd_putp(b/e, c/e, 2, 1, 1e-5); lcd_putc(OX[6]); //����ŷķ����_��ʾ�������迹,��ʾ����ŷ
  }else{
  if((showk==10)||(showk==13)||(showk==15)||(showk==18)){
	C = -e/c/w;
	L = +c/e/w;
    if(L<-2e-8) { lcd_putp(C, 0,2,4, 1e-14); lcd_putc('F'); } //��ʾCֵ,��ʾ��pF
    else        { lcd_putp(L, 0,2,2, 5e-9 ); lcd_putc('H'); } //��ʾLֵ,��ʾ��uH
  }else{
    lcd_putc(10);lcd_puts("=");
    lcd_putf(atan2(c,b)*180/3.1415926,0,0); //��ʾ�Ƕ�
    lcd_puts("    ");
  }

  }
 }
 return;
}

  if(binLian==20){ //������·����ֵ
  if(Rang_idx==3){
  a = (b*b+c*c)/a;
  TR += b/a, TX += c/a; //��·���ർ��
  }
  if(Rang_idx==0){
  TR += b/a, TX += c/a; //��·�����迹
  }
  return;
  }

  if(binLian==21){ //�Ƿ�Ӧ������
    isQ = (isQ+1)%2;
    return;
  }

  w = 2*3.1415926*Actual_Frq;
  if(isQ){ //�Զ��ֶ�
  if(Rang_idx==0){ 
    b -= cs.QRs[Frq_idx]*a, c -= cs.QXs[Frq_idx]*a;//��·����
  }
  if(Rang_idx>1){
    a = (b*b+c*c)/a;	
    b -= cs.QRo[Frq_idx]*a, c -= cs.QXo[Frq_idx]*a; //��·����
    a = (b*b+c*c)/a;
 }

  if(ATA!=0){
  if(ATD==0) { if(Rang_idx>1)ATE=1;else ATE=0;  } //��Ϊ100Hz
  if(ATD==1) { ATE=1;  } //��Ϊ1kHz
  if(ATD==2) { ATE=0;  } //��Ϊ7.8125kHz
  }else      { if(Rang_idx>1)ATE=1;else ATE=0; } 

  if(frqB[Frq_idx]==100)  LCR[0]=2e3,LCR[1]=2e-1,LCR[2]=2e7,d2=1;
  if(frqB[Frq_idx]==977)  LCR[0]=2e2,LCR[1]=2e-2,LCR[2]=2e7,d2=2;
  if(frqB[Frq_idx]==7813) LCR[0]=2e1,LCR[1]=2e-3,LCR[2]=2e6,d2=3;

  if(ATE){ //����
    e = (b*b+c*c)/a;
	C = -c/e/w;
	L = +e/c/w;
    if(C>-2e-12) { ATC=1; } 
    else         { ATC=2; } 
    if(fabs(e/b)<fabs(e/c)) { ATC=3; } 
  }else{ //����
    e = a;
	C = -e/c/w;
	L = +c/e/w;
    if(L<-2e-8)  { ATC=1; } 
    else         { ATC=2; } 
    if(fabs(b/e)>fabs(c/e)) { ATC=3; } 
   }


  qq = fabs(c/b); //����Q
  if(!b||qq>999) qq = 999;
  dd = fabs(b/c); //����D
  if(!c||dd>=10) dd = 9.999;

  if(ATA!=0){ATC=ATA;}


  lcd_goto1(0);
  if(ATC==0){

  }else if(ATC==1){
  if(ATE){ //����
  lcd_puts("Cp"); 
    if(C>-2e-12) { 
    if(fabs(C)<LCR[1]) {lcd_putp(C, 0,2,4, 1e-14); lcd_putc('F');  //��ʾ����Cֵ,��ʾ��pF
    }else{lcd_puts(" C>");lcd_puts(lcr[d2]);lcd_puts("mF");}
    } 
    else         { lcd_puts(" ---OL---"); } 
  }else{ //����
  lcd_puts("Cs"); 
    if(L<-2e-8)  {
    if(fabs(C)<LCR[1]) {lcd_putp(C, 0,2,4, 1e-14); lcd_putc('F');  //��ʾ����Cֵ,��ʾ��pF
    }else{lcd_puts(" C>");lcd_puts(lcr[d2]);lcd_puts("mF");}
    } 
    else         { lcd_puts(" ---OL---"); } 
  }

  }else if(ATC==2){
  if(ATE){ //����
  lcd_puts("Lp"); 
    if(C>-2e-12) { lcd_puts(" ---OL---"); } 
    else         {
    if(fabs(L)<LCR[0]) { lcd_putp(L, 0,2,2, 5e-9 ); lcd_putc('H');   //��ʾ����Lֵ,��ʾ��uH
    }else{lcd_puts(" L>");lcd_puts(lcr[d2-1]);lcd_puts(" H");}
    } 
  }else{ //����
  lcd_puts("Ls"); 
    if(L<-2e-8)  { lcd_puts(" ---OL---"); } 
    else         {
    if(fabs(L)<LCR[0]) { lcd_putp(L, 0,2,2, 5e-9 ); lcd_putc('H');   //��ʾ����Lֵ,��ʾ��uH
    }else{lcd_puts(" L>");lcd_puts(lcr[d2-1]);lcd_puts(" H");}
    } 
  }


  }else if(ATC==3){
  if(ATE){ //����
  lcd_puts("Rp"); 
   if(fabs(e/b)<LCR[2]) {  lcd_putp(e/b, 0,2,1, 1e-4); lcd_putc(OX[6]);} //��ʾ����
   else                 { lcd_puts(" R>");lcd_puts(lcr[d2+3]);lcd_puts(" M");lcd_putc(OX[6]); } 
  }else{ //����
  lcd_puts("Rs"); 
   if(fabs(b/e)<LCR[2]) { lcd_putp(b/e, 0,2,1, 1e-4); lcd_putc(OX[6]);} //��ʾ����
   else                 { lcd_puts(" R>");lcd_puts(lcr[d2+3]);lcd_puts(" M");lcd_putc(OX[6]); } 

  }
 }

  lcd_goto2(0);
  if(REL) {
  lcd_putc(4);lcd_puts("=");
  if(ATC==0){
  }else if(ATC==1){if(RELDAT==0){RELDAT=C;RELOUT=0;}else{RELOUT=(C-RELDAT)/(RELDAT*0.01);}
  }else if(ATC==2){if(RELDAT==0){RELDAT=L;RELOUT=0;}else{RELOUT=(L-RELDAT)/(RELDAT*0.01);}
  }else if(ATC==3){
  if(ATE){ //����
  if(RELDAT==0){RELDAT=e/b;RELOUT=0;}else{RELOUT=((e/b)-RELDAT)/(RELDAT*0.01);}
  }else{ //����
  if(RELDAT==0){RELDAT=b/e;RELOUT=0;}else{RELOUT=((b/e)-RELDAT)/(RELDAT*0.01);}
  }
  }
   if(RELOUT<-99.9)RELOUT=-100;
   if(RELOUT>99.9)RELOUT=100;

  if (fabs(RELOUT)==100){
  if (RELOUT>99.9) lcd_puts("   OL  % "); else lcd_puts("-  OL  % ");
  }else{
   RELTMP=RELOUT*1000+0.5;
   lcd_putf(RELTMP/1000.0,0,0); lcd_puts("% ");
  }
  if(!ATA){ATA=ATC;}

}else{
  RELDAT=0;
        if(ATB==0){
  lcd_puts("Q="); 
  lcd_putf(qq,0,0);lcd_puts("  "); //��ʾQ
  }else if(ATB==1){
  lcd_puts("D="); 
  lcd_putf(dd,0,0); lcd_puts("  ");//��ʾD
  }else if(ATB==2){
  lcd_putc(10);lcd_putc('=');
  lcd_putf(atan2(c,b)*180/3.1415926,0,0);lcd_puts("  "); //��ʾ�Ƕ�
  }else if(ATB==3){
  if(ATC==3){
  if(ATE){ //����
  lcd_puts("Xp");
   if(fabs(e/c)<LCR[2]) { lcd_putf(e/c, 1, 1e-4); lcd_putc(OX[6]);} //��ʾ�翹
   else                 { lcd_puts(" X>");lcd_puts(lcr[d2+3]);lcd_puts("M");lcd_putc(OX[6]); } 
  }else{ //����
  lcd_puts("Xs");
   if(fabs(c/e)<LCR[2]) { lcd_putp(c/e, b/e, 0, 1, 1e-4); lcd_putc(OX[6]);} //��ʾ�翹
   else                 { lcd_puts(" X>");lcd_puts(lcr[d2+3]);lcd_puts("M");lcd_putc(OX[6]); } 

  }
  }else{
  if(ATE){ //����
  lcd_puts("Rp");
   if(fabs(e/b)<LCR[2]) {lcd_putf(e/b, 1, 1e-4); lcd_putc(OX[6]);} //��ʾ����
   else                 { lcd_puts(" R>");lcd_puts(lcr[d2+3]);lcd_puts("M");lcd_putc(OX[6]); } 
  }else{ //����
  lcd_putc(11);lcd_putc('R');
   if(fabs(b/e)<LCR[2]) { lcd_putp(b/e, c/e, 0, 1, 1e-4); lcd_putc(OX[6]);} //��ʾ����
   else                 { lcd_puts(" R>");lcd_puts(lcr[d2+3]);lcd_puts("M");lcd_putc(OX[6]); } 
  }
  }
 }
}


  lcd_goto1(11);
  if(ATA==0||ATD==0){
  if(ATA==0) lcd_puts(" AUTO");
  if(ATA==1) lcd_puts(" AU-C");
  if(ATA==2) lcd_puts(" AU-L");
  if(ATA==3) lcd_puts(" AU-R");
  }else{
  if(ATE) lcd_puts(" P "); else lcd_puts(" S ");

  if(frqB[Frq_idx]==100)  lcd_putc('A');
  if(frqB[Frq_idx]==977)  lcd_putc('B');
  if(frqB[Frq_idx]==7813) lcd_putc('C');
  lcd_putc(Rang_idx+49); //��ʾ����
}
  lcd_goto2(11);
  lcd_puts(feqc[Frq_idx]);
  }else{
  //��ʾ������Ϣ
  lcd_goto1(0);
  if(isQ)     lcd_putc('Z');  else lcd_putc('H'); //��ʾ�Զ��ֶ�
  if(binLian) lcd_putc('p');  else lcd_putc('s'); //��ʾ��������

  lcd_goto2(0);     //��ʾƵ��
  if(frqB[Frq_idx]==100)  lcd_putc('A');
  if(frqB[Frq_idx]==977)  lcd_putc('B');
  if(frqB[Frq_idx]==7813) lcd_putc('C');
  lcd_putc(Rang_idx+49); //��ʾ����
  if(yc1&&Rang_idx!=3)     { lcd_goto1(2); lcd_puts(" Overflow,high");lcd_goto2(2); lcd_puts("         ");} //δ֪�������
  if(yc2&&Rang_idx!=0)     { lcd_goto1(2); lcd_puts(" Overflow,low ");lcd_goto2(2); lcd_puts("         ");} //δ֪�������
  if(yc1||yc2){return; } //��2�����

  //��ѧ����ʾ  if(!a) { lcd_cls(); lcd_puts("DIV 0"); return; }

  if(binLian){ //����
    e = (b*b+c*c)/a;
    lcd_goto1(2);
	lcd_putf(e/b, 1, 1e-4); //��ʾ�������迹,��ʾ����ŷ
	lcd_putf(e/c, 1, 1e-4); //��ʾ�������迹,��ʾ����ŷ
    lcd_goto2(2);
	C = -c/e/w;
	L = +e/c/w;
    if(C>-2e-12) { lcd_putf(C, 4, 1e-14); lcd_putc('F'); } //��ʾ����Cֵ,��ʾ��pF
    else         { lcd_putf(L, 2, 5e-9 ); lcd_putc('H'); } //��ʾ����Lֵ,��ʾ��uH
  }else{ //����
    e = a;
    lcd_goto1(2);
	lcd_putp(b/e, c/e, 1, 1, 1e-4); lcd_putc(OX[6]); //����ŷķ����_��ʾ�������迹,��ʾ����ŷ
    lcd_goto2(2);
	C = -e/c/w;
	L = +c/e/w;
    if(L<-2e-8) { lcd_putf(C, 4, 1e-14); lcd_putc('F'); } //��ʾCֵ,��ʾ��pF
    else        { lcd_putf(L, 2, 5e-9 ); lcd_putc('H'); } //��ʾLֵ,��ʾ��uH
  }


  c = fabs(c/b); //����Q
if(!b||c>999) c = 999;
  lcd_putf(c,0,0); //��ʾQ
}
}


main(){
 uchar i=0,s=0,key=0;
 uchar dispN=0; //��ʾɨ������
 uchar spkN=0;  //����������ʱ��
 uint nn=0;
 int kn=0;
 char mo=0; //�༭��ʾ����
 lcd_init(); //��ʼ��LCD
 lcd_cur0(); //���ع��
 lcd_B(0, 0x40, 0); //����ŷķ����_����CGRAM��ַ,дָ��
 for(i=0;i<40;i++)lcd_B(1, 0x00+path[i], 0); //����ŷķ����_����������CGRAM��ַ��
 lcd_goto1(0);//����ŷķ����_��궨λ
 lcd_puts("LCR 3.0");
 lcd_goto2(0);
 lcd_puts("XJW Putian,2012");
 delay2(500); //������ʱ
 eepEn= 12345;
 cs_RW(0);   //��EEPROM

 Init();

 while(1){
  //��ʾdisp
  nn++;
  //ɨ�����
  key = ~P3;

  if(key&&kn<255) kn++; else kn=0;
  for(i=0;key;i++) key/=2;
  key=i;

  if(menu==7&&key<3)   //У׼��ֵ����
   {   if(kn==4) spkN=10,kn=-14; else key=0;   }
  else  //������һ��ʱ���key����Ч��spkN����ʱ������
   {  if(kn==4) spkN=10; else key=0;  }

  //�˵�ϵͳ
  if(key==8){//�˵���
    lcd_cls();    
	lcd_puts(" LCR Set Options");
    lcd_goto2(0); 
	lcd_puts(" Exit:X  Ver:5.5");
    menu=0; key=0; mo=0; pau=0;
    for(i=0;i<3;i++)OX[i]=3.1415;
    delay(10000);
  }

  if(menu>=1 && menu<=3){
    if(key==7) setRng2(); //���̲���
    if(key==6) setF(-1);  //����Ƶ��
    delay(10000);
  }

  if(menu==0){ //��ʾ���̺Ͳ˵�
    if(key) lcd_cls();
	if(key>=1 && key<=7) {menu = key, menu2 = 0;if (key==1)isQ=1;}
	key = 0; 
    DDS3 = 1;
    delay(10000);
  }

  if(menu==1)  { //LCR����    
	  if(key==1) if(isQ)
	    { ATA++;
	      ATD=0;
	      REL=0;
	      if(ATA>3) {ATA=0;}
	    }else{binLian = (binLian+1)%2;} //L��C��R ģʽ

    if(key==2) if(isQ){ ATB++;REL=0;if(ATB>3){ATB=0;}} //�����л�
    if(key==3) if(!isQ||ATA==0){showR(21,0);REL=0;}else{ ATD++;REL=0;if(ATD>2){ATD=0;}} //�Զ��ֶ��л���������������ģʽѡ��
    if(key==4) {C_Done(0);} //����

    OX[7]++;
    if(OX[7]>15){OX[7]=0;showR(binLian,0);}
    delay(12000);
  }

  if(key==5) {REL = (REL+1)%2;spkN=0;}

  if(menu==2){
    lcd_goto1(0);
    lcd_puts("up:");  lcd_putc(Vxy[4]+48);
    lcd_puts(" dw:"); lcd_putc(Vxy[5]+48);
    delay(10000);
  }

  if(menu==3){ //�ֶ�����
    pau = 1;
    if(key==1) setGain2();//�������
    if(key==2) { };
    if(key==3) K3=~K3;    //�л����±�
    if(key==4) set902();  //��λ��ת
    lcd_goto1(0);
    lcd_puts(" xw="); lcd_putc(xw+48);      //��λ������
    lcd_puts(" K3="); lcd_putc(K3?49:48);   //K3״̬
    lcd_puts(" Ga="); lcd_putc(Gain_idx+48); //����������
    lcd_goto2(0);
    if(nn%32==0) lcd_int(getAD10(),5);
    delay(10000);
  }

  if(menu>=4 && menu<=5){delay(10000);}

  if(menu==6){ //Ƶ������
    if(key==1) cs.feq += 1; //X����
    if(key==2) cs.feq -= 1; //R����
    if(key==3) { cs_RW(1); setF(Frq_idx); } //L������
    if(OX[0]!=cs.feq){ lcd_cls();OX[0]=cs.feq;
    lcd_goto1(0); lcd_puts("Feq correct");
    lcd_goto2(0); lcd_putf(cs.feq,0,0);}
	delay(10000);
  }

  if(menu==7){ //У׼
 	code char *csR[19]   = {
     "Z0 :000", "Z1 :000", "Z2 :000",
     "R1 : 40", "R1X: 40",
     "ak : 25",
     "R2 : 1k", "R2X: 1k",
     "R3 :10k", "R3X:10k",
     "R4 :.1M", "R4b:.1M", "R4X:.1M",
     "G1 : 3k", "G1X: 3k",
     "G2 : 9k", "G2b: 9k", "G2X: 9k",     
     "phX: 1k"
     };
    char *p,bc=1,feqD=1,rngD=1;
	static char kc=0;
	isQ = 0;
    lcd_goto2(5);
    if(menu2==0)  p = cs.zo+0,      feqD=0, rngD=0; //100Hz���У׼,����20kŷ����
    if(menu2==1)  p = cs.zo+1,      feqD=1, rngD=0; //1kHz���У׼,����20kŷ����
    if(menu2==2)  p = cs.zo+2,      feqD=2, rngD=0; //7.8kHz���У׼,����20kŷ����
    if(menu2==3)  p = cs.R+0,       feqD=1, rngD=0; //VI�任��ģֵУ׼,����40ŷ
    if(menu2==4)  p = cs.J+0,       feqD=2, rngD=0; //40R��λ
    if(menu2==5)  p = &cs.ak,       feqD=1, rngD=0; //ADУ׼
    if(menu2==6)  p = cs.R+1,       feqD=1, rngD=1; //VI�任��ģֵУ׼,����1kŷ
    if(menu2==7)  p = cs.J+1,       feqD=2, rngD=1; //1k��λ
    if(menu2==8)  p = cs.R+2,       feqD=1, rngD=2; //VI�任��ģֵУ׼,����10kŷ
    if(menu2==9)  p = cs.J+2,       feqD=2, rngD=2; //10k��λ
    if(menu2==10) p = cs.R+3,       feqD=1, rngD=3; //VI�任��ģֵУ׼,����100kŷ
    if(menu2==11) p = &cs.R4b,      feqD=2, rngD=3; //7.8kHzƵ��100k������У׼
    if(menu2==12) p = cs.J+3,       feqD=2, rngD=3; //100k��λ
    if(menu2==13) p = &cs.g1,       feqD=1, rngD=1; //�˷�3������У׼,����3.3kŷ����
    if(menu2==14) p = &cs.j1,       feqD=2, rngD=1; //3������λ
    if(menu2==15) p = &cs.g2,       feqD=1, rngD=1; //�˷�9������У׼,����10kŷ����
    if(menu2==16) p = &cs.G2b,      feqD=2, rngD=1; //7.8kHzƵ��9��������У׼
    if(menu2==17) p = &cs.j2,       feqD=2, rngD=1; //9������λ
    if(menu2==18) p = &cs.phx,      feqD=1, rngD=1; //1kHz������λУ׼,����1kŷ����
    if(menu2<3) DDS3=0; else DDS3=1;
    if(key==1) *p += bc; //X����
    if(key==2) *p -= bc; //R����
    if(key==3) { cs_RW(1); setF(Frq_idx); } //L������
    if(key==4) *p = 0;   //C�����
	if(key==5) { if(menu2==0) menu2=18; else menu2--; mo=0; }
	if(key==6) { if(menu2==18)menu2=0;  else menu2++; mo=0; }
	if(key==7) mo = (mo+1)%2;
	if(key==4){ //�ָ���Ĭ��ֵ
	  if(++kc==5){
	   kc = 0;
       cs.j1 = 36, cs.j2 = 34;
       cs.g1 = 0,  cs.g2 = 0;
       cs.zo[0] = 76;
       cs.zo[1] = 76;
       cs.zo[2] = 70;
       cs.J[0] = cs.J[1] = 0, cs.J[2] = 4, cs.J[3] = 48;
       cs.R[0] = cs.R[1] = cs.R[2] = cs.R[3] = 0;
       cs.R4b = cs.G2b = cs.phx = cs.feq = 0;
	   cs.ak = 0;
	   for(i=0;i<3;i++) cs.QRs[i] = cs.QXs[i] = cs.QRo[i] = cs.QXo[i] = 0;
	  }
	}
	else { if(key) kc=0; }
	//��ʾ
	if(mo){
     OX[1]=3.1415;OX[2]=3.1415;
	 if(feqD!=Frq_idx) setF(feqD);
	 if(rngD!=Rang_idx)  setRng(rngD);
     if(menu2>=3&&menu2<=7) showR(0,0);
     else showR(1,0);
	}else{
	 if(feqD!=Frq_idx) setF(feqD);
	 if(rngD!=Rang_idx)  setRng(rngD);
     if(menu2>=3&&menu2<=7) showR(0,menu2+1);
     else showR(1,menu2+1);
    if(OX[1]!=*p||OX[2]!=menu2){ OX[1]=*p;OX[2]=menu2;
	  lcd_cls();
      lcd_goto1(0); lcd_puts(csR[menu2]);lcd_puts(" X:+ R:-"); //�����������
      lcd_goto2(0); lcd_int(*p,3);     //�������ֵ
      }
     delay(10000);
    }
  }


if (menu!=7){if(spkN) spkN--, spk=0; else spk=1;} //���̷���
 

 }//while end

}
//==========================================================================
