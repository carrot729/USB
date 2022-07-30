/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：09_active_buzzer.c
#  版本：V2.0
#  author: zhulin
#  说明：有源蜂鸣器实验
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_BuzzerPin      0  // 有源蜂鸣器管脚定义

int main(void)
{
	//初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}	
	pinMode(makerobo_BuzzerPin,  OUTPUT); // 有源蜂鸣器设置为输出模式
    // 无限循环
	while(1){
			digitalWrite(makerobo_BuzzerPin, LOW);  // 蜂鸣器为低电平触发，所以使能蜂鸣器让其发声
			delay(100);                             // 延时100ms
			digitalWrite(makerobo_BuzzerPin, HIGH); // 蜂鸣器设置为高电平，关闭蜂鸟器
			delay(100);	                            // 延时100ms
	}
	return 0;
}
