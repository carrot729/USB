/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：33_tilt_switch.c
#  版本：V2.0
#  author: zhulin
#  说明：循迹传感器实验
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_TrackSensorPin    0   // 循迹传感器PIN管脚

// 主函数
int main(void)
{
	//初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}
	// 设置循迹传感器管脚为输入模式
	pinMode(makerobo_TrackSensorPin, INPUT);  
    
	// 无限循环
	while(1)
	{
		// 检测到循迹模块是否检测到白色线
		if(digitalRead(makerobo_TrackSensorPin) == LOW)
		{
			printf("Makerobo White line is detected\n");  // 检测到白色线
		}	
		else{
			printf("...Makerobo Black line is detected\n");  // 检测到黑色线
			delay(100);                                      // 延时100ms
		}
	}
	return 0;
}

