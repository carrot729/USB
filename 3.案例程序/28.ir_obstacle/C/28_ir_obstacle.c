/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：28_ir_obstacle.c
#  版本：V2.0
#  author: zhulin
#  说明：红外避障传感器模块
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_ObstaclePin      0   // 红外避障传感器管脚PIN

// 中断函数
void makerobo_myISR(void)
{
	printf(" *****************************\n"); // 打印出遇到障碍物
	printf(" Makerobo Detected Barrier !\n"); // 打印出遇到障碍物
	printf(" ------------------------------\n"); // 打印出遇到障碍物
}

// 主程序
int main(void)
{
	// 初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !\n");
		return 1; 
	}
	//初始化中断函数，当makerobo_ObstaclePin管脚为0，使能中断
	if(wiringPiISR(makerobo_ObstaclePin, INT_EDGE_FALLING, &makerobo_myISR) < 0){
		printf("Unable to setup ISR !!!\n"); //初始化识别，弹出错误信息
		return 1;
	}
	// 无限循环
	while(1){
		;
	}
	return 0;
}

