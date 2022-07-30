/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：26_rotary_encoder.c
#  版本：V2.0
#  author: zhulin
#  说明：旋转编码器实验
*****************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#define  makerobo_RoAPin    0    // 旋转编码器CLK管脚
#define  makerobo_RoBPin    1    // 旋转编码器DT管脚
#define  makerobo_SWPin     2    // 旋转编码器SW管脚

static volatile int makerobo_globalCounter = 0 ;  //计数器值

unsigned char makerobo_flag;                      // 是否发生旋转标志位
unsigned char makerobo_Last_RoB_Status;           // DT 状态
unsigned char makerobo_Current_RoB_Status;        // CLK 状态

// 中断函数，当SW管脚为0，使能中断
void btnISR(void)
{
	makerobo_globalCounter = 0; // 给计数器赋0
}

// 旋转编码方向位判断函数
void makerobo_rotaryDeal(void)
{
	makerobo_Last_RoB_Status = digitalRead(makerobo_RoBPin);

	while(!digitalRead(makerobo_RoAPin)){   // 判断CLK管脚的电平变化来区分方向
		makerobo_Current_RoB_Status = digitalRead(makerobo_RoBPin);
		makerobo_flag = 1; // 发生旋转标记
	}

	if(makerobo_flag == 1){  // 标记位为1 发生了旋转
		makerobo_flag = 0;    // 复位标记位
		if((makerobo_Last_RoB_Status == 0)&&(makerobo_Current_RoB_Status == 1)){ // 逆时针方向，正 
			makerobo_globalCounter ++;	
		}
		if((makerobo_Last_RoB_Status == 1)&&(makerobo_Current_RoB_Status == 0)){ //顺时针方向，负
			makerobo_globalCounter --;
		}
	}
}

// 主程序
int main(void)
{
	// 初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() < 0){
		fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
		return 1;
	}

	pinMode(makerobo_SWPin, INPUT);    // 旋转编码器SW管脚,设置为输入模式 
	pinMode(makerobo_RoAPin, INPUT);   // 旋转编码器CLK管脚,设置为输入模式
	pinMode(makerobo_RoBPin, INPUT);   // 旋转编码器DT管脚,设置为输入模式

	pullUpDnControl(makerobo_SWPin, PUD_UP);  // 设置makerobo_SWPin管脚为上拉模式
  
  if(wiringPiISR(makerobo_SWPin, INT_EDGE_FALLING, &btnISR) < 0) //初始化中断函数，当SW管脚为0，使能中断
	{
		fprintf(stderr, "Unable to init ISR\n",strerror(errno));	 //初始化识别，弹出错误信息
		return 1;
	}
	
	int makerobo_tmp = 0;      // 当前状态判断

	while(1){
		makerobo_rotaryDeal(); // 调用旋转编码器方向判断函数
		if (makerobo_tmp != makerobo_globalCounter){  // 判断状态值发生改变
			printf("%d\n", makerobo_globalCounter);     // 打印出状态信息
			makerobo_tmp = makerobo_globalCounter;     // 把当前状态赋值到下一个状态，避免重复打印
		}   
	}

	return 0;
}
