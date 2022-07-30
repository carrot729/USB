/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：14_joystick_PS2.c
#  版本：V2.0
#  author: zhulin
#  说明：PS2操作杆实验
*****************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>

#define makerobo_PCF       120   // 基础管脚120
#define uchar	unsigned char

int pcf_AIN0 = makerobo_PCF + 0; // AIN0 端口
int pcf_AIN1 = makerobo_PCF + 1; // AIN1 端口
int pcf_AIN2 = makerobo_PCF + 2; // AIN2 端口

// 方向状态信息
 char *state[7] = {"home", "up", "down", "left", "right", "pressed"};

// 方向判断函数
int makerobo_direction(){
	int ain_x, ain_y, ain_b;       // X方向，Y方向，B 是否按下
	int makerobo_tmp=0;            // 状态值
	ain_x = analogRead(pcf_AIN1);  // X为AIN1端口
	ain_y = analogRead(pcf_AIN0);  // Y为AIN0端口
	ain_b = analogRead(pcf_AIN2);  // B按下为AIN2端口
 
	if (ain_y <= 30)
		makerobo_tmp = 1;		// up
	if (ain_y >= 225)
		makerobo_tmp = 2;		// down
	
	if (ain_x >= 225)
		makerobo_tmp = 4;		// left
	if (ain_x <= 30)
		makerobo_tmp = 3;		// right

	if (ain_b == 0)
		makerobo_tmp = 5;		// button 按下
	if (ain_x-125<15 && ain_x-125>-15 && ain_y-125<15 && ain_y-125>-15 && ain_b >= 60)
		makerobo_tmp = 0;		// home 位置
	
	return makerobo_tmp;
}

// 主函数
int main (void)
{
	int makerobo_tmp=0;       // 当前值
	int makerobo_status = 0;  // 状态值
	wiringPiSetup ();
	// 在基本引脚120上设置pcf8591，地址0x48
	pcf8591Setup (makerobo_PCF, 0x48);
	// 无线循环
	while(1) 
	{
		makerobo_tmp = makerobo_direction(); // 调用方向判断函数
		if (makerobo_tmp != makerobo_status) // 判断状态是否发生改变
		{
			printf("%s\n", state[makerobo_tmp]); // 打印出方向位
			makerobo_status = makerobo_tmp;      // 把当前状态赋给状态值，以防止同一状态多次打印
		}
	}
	return 0 ;
}
