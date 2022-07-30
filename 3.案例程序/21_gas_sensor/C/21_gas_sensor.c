/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：21_gas_sensor.c
#  版本：V2.0
#  author: zhulin
#  说明：烟雾传感器模块实验
*****************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>
#include <math.h>

#define		makerobo_PCF     120   // 基础管脚120
#define		makerobo_DOpin	 0     // 烟雾传感器数字IO口
#define		makerobo_Buzz	 1     // 蜂鸣器数字IO口

// 打印函数，打印出烟雾传感器信息
void makerobo_Print(int x)
{
	switch(x)
	{
		case 1:     // 安全
			printf("\n*****************\n"  );
			printf(  "*Makerobo Saft~ *\n"  );
			printf(  "*****************\n\n");
		break;
		case 0:   // 检测到烟雾
			printf("\n************************\n"  );
			printf(  "* Makerobo Danger Gas! *\n"  );
			printf(  "************************\n\n");
		break;
		default:  // 打印错误信息
			printf("\n**********************\n"  );
			printf(  "* Print value error. *\n"  );
			printf(  "**********************\n\n");
		break;
	}
}
// 主函数
int main()
{
	int makerobo_analogVal;                            // 读取模拟量值
	int makerobo_tmp, makerobo_status, makerobo_count; // 定义当前状态，上次状态，和计数值变量
	// 初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1;
	}
	//  在基本引脚120上设置pcf8591，地址0x48
	pcf8591Setup(makerobo_PCF, 0x48);

	pinMode (makerobo_DOpin,INPUT);      // 烟雾传感器数字IO口,设置为输入模式
	pinMode (makerobo_Buzz,OUTPUT);      // 蜂鸣器数字IO口,设置为输出模式
	digitalWrite(makerobo_Buzz,HIGH);   //  设置蜂鸣器为高电平，初始状态关闭蜂鸣器鸣叫

	makerobo_status = 0;               // 状态值初始化为0
	makerobo_count = 0;                // 计数器值初始化为0
    
	// 无限循环
	while(1) 
	{
		makerobo_analogVal = analogRead(makerobo_PCF + 0); // 获取AIN0 的模拟量值；
		printf("%d\n", makerobo_analogVal);                // 打印出该模拟值

		makerobo_tmp = digitalRead(makerobo_DOpin);       // 读取GAS烟雾传感器数字IO口值

		if (makerobo_tmp != makerobo_status)             // 判断状态发生改变
		{
			makerobo_Print(makerobo_tmp);               // 打印函数，打印出烟雾传感器信息
			makerobo_status = makerobo_tmp;             // 把当前状态值设置为比较状态值，避免重复打印；
		}
		// 当检测到烟雾
		if (makerobo_status == 0)                      // 状态值为0
		{
			  makerobo_count ++;                      // 计数器值累计
			  // 高低电平交替变化，让蜂鸣器发声
			if (makerobo_count % 2 == 0)              // 进行求余处理，一半为1，一半为0
				{digitalWrite(makerobo_Buzz, HIGH);}
			else
				{digitalWrite(makerobo_Buzz, LOW);}
		}
		else
		{
			makerobo_count = 0; //计数器赋0
			digitalWrite(makerobo_Buzz, HIGH);   //  设置蜂鸣器为高电平，初始状态关闭蜂鸣器鸣叫
		}
		delay (200);                            // 延时200ms 
	}
	return 0;
}
