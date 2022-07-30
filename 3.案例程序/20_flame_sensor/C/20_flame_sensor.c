/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：20_flame_sensor.c
#  版本：V2.0
#  author: zhulin
#  说明：火焰传感器实验
*****************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>
#include <math.h>

#define		makerobo_PCF     120  // 基础管脚120
#define		makerobo_DOpin	 0    // 火焰传感器管脚

// 打印信息，打印出火焰传感器的状态值
void makerobo_Print(int x)
{
	switch(x)
	{
		case 1:       // 安全
			printf("\n******************\n"  );
			printf(  "* Makerobo Saft~ *\n"  );
			printf(  "******************\n\n");
		break;
		case 0:      // 有火焰
			printf("\n******************\n"  );
			printf(  "*Makerobo  Fire! *\n"  );
			printf(  "******************\n\n");
		break;
		default:    // 错误
			printf("\n**********************\n"  );
			printf(  "* Print value error. *\n"  );
			printf(  "**********************\n\n");
		break;
	}
}
// 主函数
int main()
{
	int makerobo_analogVal;              // 模拟量变量
	int makerobo_tmp, makerobo_status;   // 模拟量状态值
	
	// 初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1;
	}
	// 在基本引脚120上设置pcf8591，地址0x48
	pcf8591Setup(makerobo_PCF, 0x48);
    // 火焰传感器数字端口DO值，设置为输入模式
	pinMode(makerobo_DOpin, INPUT);
    // 状态值
	makerobo_status = 0; 
	// 无限循环
	while(1) 
	{
		makerobo_analogVal = analogRead(makerobo_PCF + 0); // 获取AIN0上的值，读取火焰传感器值
		printf("%d\n", makerobo_analogVal);      // 打印出火焰传感器的模拟量值

		makerobo_tmp = digitalRead(makerobo_DOpin);     // 读取数字IO口的值

		if (makerobo_tmp != makerobo_status)       // 判断状态发生改变
		{
			makerobo_Print(makerobo_tmp);         // 打印出火焰信息值
			makerobo_status = makerobo_tmp;       // 把当前状态值作为下次状态值判断，避免重复打印
		} 
		delay (200);                             // 延时 200ms
	}
	return 0;
}
