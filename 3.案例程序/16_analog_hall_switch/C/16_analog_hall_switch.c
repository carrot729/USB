/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：16_analog_hall_switch.c
#  版本：V2.0
#  author: zhulin
#  说明：模拟霍尔传感器实验
*****************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>

#define makerobo_PCF       120 // 基础管脚120

// 主函数
int main (void)
{
	int makerobo_res, makerobo_tmp, makerobo_status; // 定义状态值、缓冲值和当前值变量
	wiringPiSetup ();  // wiringPi初始化
	//  在基本引脚120上设置pcf8591，地址0x48
	pcf8591Setup (makerobo_PCF, 0x48);
	makerobo_status = 0; // 设置默认状态值为0
	// 无限循环
	while(1) 
	{
		makerobo_res = analogRead(makerobo_PCF + 0);  // 获取AIN0 的模拟量值；
		printf("Current intensity of magnetic field : %d\n", makerobo_res); // 打印出磁场的电流强度值
		if (makerobo_res - 133 < 5 || makerobo_res - 133 > -5) // 判断磁场强度范围
			makerobo_tmp = 0;     // 没有磁场
		if (makerobo_res < 128) makerobo_tmp = -1; // 磁场强度为北
		if (makerobo_res > 138) makerobo_tmp =  1; // 磁场强度为南
		if (makerobo_tmp != makerobo_status)       // 磁场发生改变
		{
			switch(makerobo_tmp) // 磁场判断
			{
				case 0:    // 没有磁场
					printf("\n*****************\n"  );
					printf(  "* Magnet: None. *\n"  );
					printf(  "*****************\n\n");
					break;
				case -1:  // 磁场为北
					printf("\n******************\n"  );
					printf(  "* Magnet: North. *\n"  );
					printf(  "******************\n\n");
					break;
				case 1:  // 磁场为南
					printf("\n******************\n"  );
					printf(  "* Magnet: South. *\n"  );
					printf(  "******************\n\n");
					break;
			}
			makerobo_status = makerobo_tmp;  //把当前状态值设置为比较状态值，避免重复打印；
		}
		delay (200);      // 延时200ms 
	}
	return 0 ;
}
