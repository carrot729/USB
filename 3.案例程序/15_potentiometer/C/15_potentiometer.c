/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：15_potentiometer.c
#  版本：V2.0
#  author: zhulin
#  说明：电位器模块实验
*****************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>

#define makerobo_PCF       120 // 基础管脚120

// 主函数
int main (void)
{
	int pcf_value ;     // 定义一个变量存储AIN的输入值
	wiringPiSetup () ;   // 调用初始化函数
	// 在基本引脚120上设置pcf8591，地址0x48
	pcf8591Setup (makerobo_PCF, 0x48) ;
	while(1) // 无限循环
	{
		pcf_value = analogRead  (makerobo_PCF + 0) ;    // 获取AIN0上的值，读取电位器模拟量值；
		printf("Potentiometer Value: %d\n", pcf_value); // 打印出该值    
		analogWrite (makerobo_PCF + 0, pcf_value) ;     // 控制AOUT输出电平控制LED灯
		delay (200) ;                                   // 延时200ms
	}
	return 0 ;
}
