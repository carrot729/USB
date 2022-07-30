/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：10_reed_switch.c
#  版本：V2.0
#  author: zhulin
#  说明：干簧管传感器实验
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_ReedPin	0 //干簧管传感器管脚定义
#define makerobo_Rpin		1 //红色LED模块管脚定义
#define makerobo_Gpin		2 //绿色LED模块管脚定义

// 双色LED控制函数
void double_colorLED(char* color)
{
	pinMode(makerobo_Rpin, OUTPUT); //红色LED管脚设置为输出模式
	pinMode(makerobo_Gpin, OUTPUT); //绿色LED管脚设置为输出模式

	if (color == "makeroboRED")       //点亮红色LED灯
	{
		digitalWrite(makerobo_Rpin, HIGH);
		digitalWrite(makerobo_Gpin, LOW);
	}
	else if (color == "makeroboGREEN") //点亮绿色LED灯
	{
		digitalWrite(makerobo_Rpin, LOW);
		digitalWrite(makerobo_Gpin, HIGH);
	}
	else
		printf("Double color LED Error");
}

int main(void)
{
	//初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}
    //干簧管传感器Pin设置为输入模式
	pinMode(makerobo_ReedPin, INPUT);
	double_colorLED("makeroboGREEN");  //点亮绿色LED模块
	
	while(1){
		//干簧管传感器去除抖动处理
		if(0 == digitalRead(makerobo_ReedPin)){ //干簧管传感器检测到磁性物质
			delay(10);                 // 延时去抖动
			if(0 == digitalRead(makerobo_ReedPin)){
				double_colorLED("makeroboRED");	   // 点亮红色LED灯
				printf("Detected Magnetic Material!\n");	// 打印出检测到磁性物质
			}
		}
		else if(1 == digitalRead(makerobo_ReedPin)){ // 干簧管传感器没有检测到磁性物质
			delay(10);                               // 延时去抖动
			if(1 == digitalRead(makerobo_ReedPin)){
				while(!digitalRead(makerobo_ReedPin));
				double_colorLED("makeroboGREEN");                      // 点亮绿色LED灯
				printf("No Detected Magnetic Material!\n");           // 打印出没有检测到磁性物质
			}
		}
	}
	return 0;
}
