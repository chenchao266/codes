/*1. mouse.cԴ����*/
/***mouse.c***/
#include "dos.h"
/*�������ʾ*/
void MouseOn()
{
   _AX=0x01;
   geninterrupt(0x33);/*�������ж�*/
}
/*���������*/
void MouseOff()
{
   _AX=0x02;
   geninterrupt(0x33);
}
/*������굱ǰλ��x��y*/
void MouseSetXY(int x,int y)
{
   _CX=x;
   _DX=y;
   _AX=0x04;
   geninterrupt(0x33);
}
/*��ȡ���״̬��x��y�������λ�ã�status������������������ */
void MouseStatus(int *x,int *y,int *status)
{
   union REGS regs; /*�Ĵ�������*/
   regs.x.ax=3;
   int86(0x33,&regs,&regs);
   *x=regs.x.cx;
   *y=regs.x.dx;
   *status=regs.x.bx;
}
