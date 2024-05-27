#include"main.h"

xdata uchar CWADD1 _at_ 0x8000;   // 写指令代码地址(E1)
xdata uchar DWADD1 _at_ 0x8001;   // 写显示数据地址(E1)
xdata uchar CRADD1 _at_ 0x8002;   // 读状态字地址(E1)
xdata uchar DRADD1 _at_ 0x8003;   // 读显示数据地址(E1)

xdata uchar CWADD2 _at_ 0x8004;   // 写指令代码地址(E2)
xdata uchar DWADD2 _at_ 0x8005;   // 写显示数进地址(E2)
xdata uchar CRADD2 _at_ 0x8006;   // 读状态字地址(E2)
xdata uchar DRADD2 _at_ 0x8007;   // 读显示数据地址(E2)


uchar Command; // 指令寄存器
uchar LCDData; // 数据寄存器


// 1.写指令代码子程序(E1)
void WriteCommandE1()
{
  while(CRADD1 & 0x80);
  CWADD1 = Command;
}

// 2.写显示数据子程序(E1)
void WriteDataE1()
{
  while(CRADD1 & 0x80);
  DWADD1 = LCDData;
}

// 3.读显示数据子程序(E1)
void ReadDataE1()
{
  while(CRADD1 & 0x80);
  LCDData = DRADD1;
}

// 4.写指令代码子程序(E2)
void WriteCommandE2()
{
  while(CRADD2 & 0x80);
  CWADD2 = Command;
}

// 5.写显示数据子程序(E2)
void WriteDataE2()
{
  while(CRADD2 & 0x80);
  DWADD2 = LCDData;
}

// 6.读显示数据子程序(E2)
void ReadDataE2()
{
  while(CRADD2 & 0x80);
  LCDData = DRADD2;
}

// 初始化程序
void Init_LCD()
{
  Command = 0xe2;
  WriteCommandE1();
  WriteCommandE2();

  Command = 0xa4;
  WriteCommandE1();
  WriteCommandE2();

  Command = 0xa9;
  WriteCommandE1();
  WriteCommandE2();

  Command = 0xa0;
  WriteCommandE1();
  WriteCommandE2();

  Command = 0xc0;
  WriteCommandE1();
  WriteCommandE2();

  Command = 0xaf;
  WriteCommandE1();
  WriteCommandE2();
}

// 清屏
void Clear()
{
  uchar i,j;

  i = 0;
  do {

    Command = (i + 0xb8); // 页地址设置
    WriteCommandE1();
    WriteCommandE2();

    Command = 0x00;      // 列地址设置为"0"
    WriteCommandE1();
    WriteCommandE2();

    j = 0x50;            // 一页清 80个字节
    do {
      LCDData = 0x00;    // 显示数据为"0"
      WriteDataE1();
      WriteDataE2();
    }while(--j !=0);     // 页内字节清零循环

  }while(++i !=4);       // 页地址暂存器加一
                         // 显示区清零循环
}
void Delay0ms()		//@12.000MHz
{
	uchar i, j;

	i = 10;
	j = 10;
	do
	{
		while (--j);
	} while (--i);
}

void Delay5ms()		//@12.000MHz
{
	uchar i, j;

	i = 10;
	j = 183;
	do
	{
		while (--j);
	} while (--i);
}
void Delay500ms()		//@12.000MHz
{
	uchar i, j;

	i = 1000;
	j = 183;
	do
	{
		while (--j);
	} while (--i);
}