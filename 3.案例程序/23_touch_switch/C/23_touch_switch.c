/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：23_touch_switch.c
#  版本：V2.0
#  author: zhulin
#  说明：触摸开关传感器模块
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_TouchPin	0   // 触摸传感器管脚PIN
#define makerobo_Rpin		1   // 红色LED管脚PIN
#define makerobo_Gpin		2   // 绿色LED管脚PIN

int makerobo_tmp = 0;     // 是否有触摸判断

// 双色LED模块驱动子函数
void double_colorLED(int color)
{
	pinMode(makerobo_Gpin, OUTPUT);  //设置为输出模式
	pinMode(makerobo_Rpin, OUTPUT);  //设置为输出模式

	if (color == 1)                 //点亮红色LED灯
	{
		digitalWrite(makerobo_Rpin, HIGH);
		digitalWrite(makerobo_Gpin, LOW);
	}
	else if (color == 0)         //点亮绿色LED灯
	{
		digitalWrite(makerobo_Rpin, LOW);
		digitalWrite(makerobo_Gpin, HIGH);
	}
	else
		printf("Double color LED Error");
}

// 在终端上打印显示函数，提示释放有触摸
void makerobo_Print(int x){
	if (x != makerobo_tmp){
		if (x == 1)        // 发生触摸
			printf("Makerobo Have a touch\n");
		if (x == 0)       // 没有发生触摸
			printf("Makerobo No touch occurred\n");
		makerobo_tmp = x;  // 触摸状态值保存和下次做比较，避免重复打印
	}
}

// 主函数
int main(void)
{
	// 初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}
    //触摸传感器Pin设置为输入模式
	pinMode(makerobo_TouchPin, INPUT);
	
	while(1){
		double_colorLED(digitalRead(makerobo_TouchPin));	 // 调用双色LED模块驱动子函数
		makerobo_Print(digitalRead(makerobo_TouchPin));	     // 调用打印程序
	}
	return 0;
}
