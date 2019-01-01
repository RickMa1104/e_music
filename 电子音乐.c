#include<reg51.h>
sbit sp = P1^5;	  //������ֿ�
unsigned char hi_note=0;   //��ʱ����ֵ�߰�λ
unsigned char low_note=0;  //��ʱ����ֵ�Ͱ�λ
unsigned char i; //��������ѭ������
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
};	   //���״���
code int note[]={0x0000,0xFB03,0xFB83,
0xFC0B,0xFC43,0xFCAB,0xFD08,0xFD32,0xFD81,
0xFDC7,0xFE05,0xFE21,0xFE55,0xFE83,0xFE99,
0xFEC0}	; //�̶�Ƶ�ʶ�Ӧ�Ķ�ʱ����ֵ����
/**********************������ʱ*************************/
void delay(unsigned int n)
{
 unsigned int i,j;
 for(i=0;i<1000*n;i++)
 for(j=0;j<120;j++);

}
/************************�̶�Ƶ�ʷ�������****************************/
static void time1_isr(void)interrupt 3
{
TR1=0;
TL1=low_note;
TH1=hi_note;
sp=~sp;
TR1=1;
}
/*************************��ʱ����ʼ��****************************************/
static void time1_initialize(void)
{
EA=0;
TR1=0;
TMOD=0x10;	//���ö�ʱ��1Ϊ������ʽ1
ET1=1;	//�򿪶�ʱ��1�ж�
EA=1;	// �����ж�
}
/******************************************************************/
void singing()
{
unsigned char beat,temp;  //������ĺ���������
i=0;
do
{
temp=song[i];//ȡ����
if(temp==0)
{
TR1=0; //����Ϊ0ʱ����ִ�ж�ʱ���жϣ�ֱ�ӷ���
return;
}
beat=temp&0x0f;	//ȡ������λ������
temp=(temp>>4)&0x0f; //ȡ������λ�����̶�Ƶ��
if(temp==0)
TR1=1;	   //Ƶ��Ϊ0ʱ��ֱ�ӿ�����ʱ����ʱĬ��ʱ��Ϊ0��
else
{
hi_note=note[temp]>>8;	//ȡ�̶�Ƶ�ʵĸ߰�λ
low_note=note[temp]&0x00ff;	//ȡ�̶�Ƶ�ʵĵͰ�λ
TR1=1;	 //	 �趨�ù̶�Ƶ�ʵĶ�ʱ��ֵ�󣬿�����ʱ��
}
delay(beat);  //�̶�Ƶ���жϵĳ���ʱ�䣬������ʱ�� 
i++;   //ȡ��һ����
}
while(1);

}
/**********************************************************************/
void main(void)
{
time1_initialize();	 //��ʱ����ʼ��
do
{
singing();
}
while(1);

}