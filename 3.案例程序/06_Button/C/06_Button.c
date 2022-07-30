/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：06_Button.c
#  版本：V2.0
#  author: zhulin
#  说明：轻触开关按键实验
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_BtnPin		0   // 轻触按键PIN口
#define makerobo_Rpin		1   // 红色LED灯模块PIN
#define makerobo_Gpin		2   // 绿色LED灯模块PIN

//双色LED控制函数
void double_colorLED(char* color)
{
	pinMode(makerobo_Rpin, OUTPUT);  //设置为输出模式
	pinMode(makerobo_Gpin, OUTPUT);  //设置为输出模式
    
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
		printf("makerobo Double color LED Error");
}

int main(void)
{
	//初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}
    //轻触按键Pin设置为输入模式
	pinMode(makerobo_BtnPin, INPUT);
	double_colorLED("makeroboGREEN");
	
	while(1){
		//轻触按键去除抖动处理
		if(0 == digitalRead(makerobo_BtnPin)){      // 按键有按下
			delay(10);                         // 延时去抖动
			if(0 == digitalRead(makerobo_BtnPin)){
				double_colorLED("makeroboRED");	   // 点亮红色LED灯
				printf("Button is pressed\n"); // 打印出有按键按下
			}
		}
		else if(1 == digitalRead(makerobo_BtnPin)){ // 没有按键按下亮绿灯
			delay(10);
			if(1 == digitalRead(makerobo_BtnPin)){
				while(!digitalRead(makerobo_BtnPin));
				double_colorLED("makeroboGREEN");   // 点亮绿色LED灯
				printf("No Button is pressed\n"); // 没有按键按下
			}
		}
	}
	return 0;
}
