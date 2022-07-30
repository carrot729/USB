/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：13_rain_detector.c
#  版本：V2.0
#  author: zhulin
#  说明：雨滴探测传感器实验
*****************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>
#include <math.h>

#define		makerobo_PCF     120 // 基础管脚120
#define		makerobo_DOpin	 0   // 雨滴传感器管脚

// 打印出雨滴传感器提示信息
void makerobo_Print(int x)
{
	switch(x)
	{
		case 1:                              // 没有雨滴
			printf("\n************************\n"  );
			printf(  "* makerobo Not Raining *\n"  );
			printf(  "************************\n\n");
		break;
		case 0:                             // 有雨滴
			printf("\n**********************\n"  );
			printf(  "* makerobo Raining!! *\n"  );
			printf(  "**********************\n\n");
		break;
		default:                           // 打印数据
			printf("\n**********************\n"  );
			printf(  "* Print value error. *\n"  );
			printf(  "**********************\n\n");
		break;
	}
}
// 主函数
int main()
{
	int makerobo_analogVal;           // 定义模拟量存储变量
	int makerobo_tmp, makerobo_status;// 定义状态信息
    //初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1;
	}
	// 在基本引脚120上设置pcf8591，地址0x48
	pcf8591Setup(makerobo_PCF, 0x48);
    // 设置雨滴传感器管脚为输入模式
	pinMode(makerobo_DOpin, INPUT);
    //  默认状态值设置为0
	makerobo_status = 0; 
	// 无线循环
	while(1) 
	{
		makerobo_analogVal = analogRead(makerobo_PCF + 0); // 获取AIN0上的值
		printf("%d\n", makerobo_analogVal);                // 打印出该值 

		makerobo_tmp = digitalRead(makerobo_DOpin);        // 读取数字IO口电平，读取数字雨滴传感器DO端口

		if (makerobo_tmp != makerobo_status)               // 状态发生改变
		{
			makerobo_Print(makerobo_tmp);                 //  打印出雨滴传感器检测信息
			makerobo_status = makerobo_tmp;               // 状态值重新赋值
		}
		delay (200);                                     // 延时200ms
	}
	return 0;
}
