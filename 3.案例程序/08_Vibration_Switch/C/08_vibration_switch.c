/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：08_vibration_switch.c
#  版本：V2.0
#  author: zhulin
#  说明：振动传感器实验
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_VibratePin	0   // 震动传感器
#define makerobo_Rpin		1   // 红色LED PIN 
#define makerobo_Gpin		2   // 绿色LED PIN
int clb_tmp = 0;
//双色LED控制函数
void double_colorLED(int color)
{
	pinMode(makerobo_Rpin, OUTPUT); //设置为输出模式
	pinMode(makerobo_Gpin, OUTPUT); //设置为输出模式

	if (0 == color)             //点亮红色LED灯
	{
		digitalWrite(makerobo_Rpin, HIGH);
		digitalWrite(makerobo_Gpin, LOW);
	}
	else if (1 == color)       //点亮绿色LED灯
	{
		digitalWrite(makerobo_Rpin, LOW);
		digitalWrite(makerobo_Gpin, HIGH);
	}
	else
		printf("Makerobo Double color LED Error"); // 双色LED错误
}

// 打印信息,打印出振动传感器的状态	
void makerobo_Print(int x){
	if (x != clb_tmp){
		if (x == 0)
			printf("...Makerobo ON\n");
		if (x == 1)
			printf("Makerobo OFF..\n");
		clb_tmp = x;
	}
}

int main(void)
{
	int clb_status = 0;  //状态值
	int clb_tmp = 0;     //比较值
	int clb_value = 1;   //当前值
	//初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}
    //振动传感器Pin设置为输入模式
	pinMode(makerobo_VibratePin, INPUT);
	
	while(1){
		clb_value = digitalRead(makerobo_VibratePin);  //获取振动传感的值
		if (clb_tmp != clb_value){                     //值发生改变
			clb_status ++;                             //振动传感器状态加1
			if (clb_status > 1){                       //状态发生改变判断
				clb_status = 0;                        //状态值复位
			}
			double_colorLED(clb_status);	          //控制双色LED模块
			makerobo_Print(clb_status);               //打印出状态
			delay(1000);                              //延时1s
		}
	}
	return 0;
}
