/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：11_photo_interrupter.c
#  版本：V2.0
#  author: zhulin
#  说明：U型光电传感器实验
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_LBPin		0  // U型光电传感器管脚定义
#define makerobo_Rpin		1  // 红色LED模块管脚定义
#define makerobo_Gpin		2  // 绿色LED模块管脚定义

// 双色LED控制函数
void double_colorLED(int color)
{
	pinMode(makerobo_Rpin, OUTPUT); // 红色LED管脚设置为输出模式
	pinMode(makerobo_Gpin, OUTPUT); // 绿色LED管脚设置为输出模式
	
	if (color == 0){                // 点亮红色LED灯
		digitalWrite(makerobo_Rpin, HIGH);
		digitalWrite(makerobo_Gpin, LOW);
	}
	else if (color == 1){          // 点亮绿色LED灯
		digitalWrite(makerobo_Rpin, LOW);
		digitalWrite(makerobo_Gpin, HIGH);
	}
}
// 有黑色物体挡住时，打印出提示信息
void makerobo_Print(int x){
	if ( x == 0 ){
		printf("makerobo Light was blocked\n");
	}
}
// 主函数
int main(void){
   
    //初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}
    //U型光电传感器Pin设置为输入模式
	pinMode(makerobo_LBPin, INPUT);
	int clb_temp;
	while(1){
		//U型光电传感器有无物体挡住信息检测
		if ( digitalRead(makerobo_LBPin) == 0 ){  
			clb_temp = 1;
		}
		if ( digitalRead(makerobo_LBPin) == 1 ){
			clb_temp = 0;
		}
		double_colorLED(clb_temp); // 控制双色LED灯显示
		makerobo_Print(clb_temp);  // 信息打印
	}
	return 0;
}
