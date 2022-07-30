/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：07_tilt_switch.c
#  版本：V2.0
#  author: zhulin
#  说明：倾斜传感器实验
*****************************************/

#include <wiringPi.h>
#include <stdio.h>

#define makerobo_TiltPin	0 //倾斜传感器管脚定义
#define makerobo_Rpin		1 //红色LED模块管脚定义
#define makerobo_Gpin		2 //绿色LED模块管脚定义

//双色LED控制函数
void double_colorLED(char* color)
{
	pinMode(makerobo_Rpin, OUTPUT); //设置为输出模式
	pinMode(makerobo_Gpin, OUTPUT); //设置为输出模式

	if (color == "makeroboRED")     //点亮红色LED灯
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
    //倾斜传感器Pin设置为输入模式
	pinMode(makerobo_TiltPin, INPUT);
	double_colorLED("makeroboGREEN");
	
	while(1){
		//倾斜传感器去除抖动处理
		if(0 == digitalRead(makerobo_TiltPin)){     //倾斜传感器发生倾斜
			delay(10);                     // 延时去抖动
			if(0 == digitalRead(makerobo_TiltPin)){
				double_colorLED("makeroboRED"); // 点亮红色LED灯
				printf("Tilt!\n");         // 打印出出现倾斜
			}
		}
		else if(1 == digitalRead(makerobo_TiltPin)){ // 倾斜传感器没有发生倾斜
			delay(10);
			if(1 == digitalRead(makerobo_TiltPin)){
				while(!digitalRead(makerobo_TiltPin));
				double_colorLED("makeroboGREEN");          // 点亮绿色LED灯
				printf("No Tilt!\n");         // 打印出出现倾斜
			}
		}
	}
	return 0;
}
