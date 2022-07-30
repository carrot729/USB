/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：05_laser.c
#  版本：V2.0
#  author: zhulin
#  说明：激光传感器实验
#  注意事项： 不要直接用眼睛看激光光束，
#  对眼睛不好！
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_LaserPin    0  //激光传感器管脚定义

int main(void)
{
	//初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}
	
	pinMode(makerobo_LaserPin, OUTPUT); // 激光传感器设置为输出模式

	while(1){
		digitalWrite(makerobo_LaserPin, HIGH); //打开激光传感器
		delay(500);                            //延时500mm
		digitalWrite(makerobo_LaserPin, LOW);  //关闭激光传感器
		delay(500);                            //延时500mm
	}
	return 0;
}
