/*1. mouse.c源程序*/
/***mouse.c***/
#include "dos.h"
/*鼠标光标显示*/
void MouseOn()
{
   _AX=0x01;
   geninterrupt(0x33);/*产生软中断*/
}
/*鼠标光标隐藏*/
void MouseOff()
{
   _AX=0x02;
   geninterrupt(0x33);
}
/*设置鼠标当前位置x和y*/
void MouseSetXY(int x,int y)
{
   _CX=x;
   _DX=y;
   _AX=0x04;
   geninterrupt(0x33);
}
/*读取鼠标状态，x和y返回鼠标位置，status返回鼠标击键操作代码 */
void MouseStatus(int *x,int *y,int *status)
{
   union REGS regs; /*寄存器变量*/
   regs.x.ax=3;
   int86(0x33,&regs,&regs);
   *x=regs.x.cx;
   *y=regs.x.dx;
   *status=regs.x.bx;
}
