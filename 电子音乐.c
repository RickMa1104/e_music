#include<reg51.h>
sbit sp = P1^5;	  //输出音乐口
unsigned char hi_note=0;   //定时器初值高八位
unsigned char low_note=0;  //定时器初值低八位
unsigned char i; //乐谱音符循环变量
code unsigned song[] = {
0x42,0x01,0x41,0x54,0x44,0x74,
0x64,0x42,0x01,0x41,0x54,0x44,
0x84,0x74,0x74,0x42,0x01,0x41,
0xB4,0x84,0x74,0x64,0xC4,0xA2,
0x01,0xA1,0x94,0x74,0x84,0x74,
0x74,
0x42,0x01,0x41,0x54,0x44,0x74,
0x64,0x42,0x01,0x41,0x54,0x44,
0x84,0x74,0x74,0x42,0x01,0x41,
0xB4,0x84,0x74,0x64,0xC4,0xA2,
0x01,0xA1,0x94,0x74,0x84,0x74,
0x74,0x00
};	   //乐谱代码
code int note[]={0x0000,0xFB03,0xFB83,
0xFC0B,0xFC43,0xFCAB,0xFD08,0xFD32,0xFD81,
0xFDC7,0xFE05,0xFE21,0xFE55,0xFE83,0xFE99,
0xFEC0}	; //固定频率对应的定时器初值数组
/**********************节拍延时*************************/
void delay(unsigned int n)
{
 unsigned int i,j;
 for(i=0;i<1000*n;i++)
 for(j=0;j<120;j++);

}
/************************固定频率方波控制****************************/
static void time1_isr(void)interrupt 3
{
TR1=0;
TL1=low_note;
TH1=hi_note;
sp=~sp;
TR1=1;
}
/*************************定时器初始化****************************************/
static void time1_initialize(void)
{
EA=0;
TR1=0;
TMOD=0x10;	//设置定时器1为工作方式1
ET1=1;	//打开定时器1中断
EA=1;	// 打开总中断
}
/******************************************************************/
void singing()
{
unsigned char beat,temp;  //定义节拍和音调变量
i=0;
do
{
temp=song[i];//取音符
if(temp==0)
{
TR1=0; //音符为0时，不执行定时器中断，直接返回
return;
}
beat=temp&0x0f;	//取出低四位节拍数
temp=(temp>>4)&0x0f; //取出高四位音调固定频率
if(temp==0)
TR1=1;	   //频率为0时，直接开启定时，定时默认时间为0；
else
{
hi_note=note[temp]>>8;	//取固定频率的高八位
low_note=note[temp]&0x00ff;	//取固定频率的低八位
TR1=1;	 //	 设定好固定频率的定时初值后，开启定时器
}
delay(beat);  //固定频率中断的持续时间，即节拍时间 
i++;   //取下一音符
}
while(1);

}
/**********************************************************************/
void main(void)
{
time1_initialize();	 //定时器初始化
do
{
singing();
}
while(1);

}