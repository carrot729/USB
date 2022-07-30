/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：18_sound_sensor.c
#  版本：V2.0
#  author: zhulin
#  说明：声音传感器模块实验
*****************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>

#define makerobo_PCF       120  // 基础管脚120

// 主函数
int main (void)
{
	int sound_value;    //声音传感器AO模拟变量值
	wiringPiSetup ();   // 调用wiringPi初始化函数
	// 在基本引脚120上设置pcf8591，地址0x48
	pcf8591Setup (makerobo_PCF, 0x48);

	while(1) // 无限循环
	{
		sound_value = analogRead  (makerobo_PCF + 0); // 获取AIN0上的值,读取声音传感器值
		printf("value: %d\n", sound_value);  //打印出该值  
		if (sound_value < 140){               // 如果声音传感器读取值小于80
			printf("Makerobo Voice In!! \n");         // 打印出提示信息
		}
		delay(100);                          // 延时100ms
	}
	return 0;
}
