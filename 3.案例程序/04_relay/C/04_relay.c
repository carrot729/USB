/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：04_relay.c
#  版本：V2.0
#  author: zhulin
# 说明：继电器模块实验
*****************************************/
#include <wiringPi.h>
#include <stdio.h>

#define makerobo_RelayPin      0   // 继电器GPIO管脚

//-------------主程序-----------------
int main(void)
{
    //初始化连接失败时，将消息打印到屏幕	
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}	
	// 继电器管脚设置为输出模式
	pinMode(makerobo_RelayPin, OUTPUT);

	while(1){
			digitalWrite(makerobo_RelayPin, HIGH); // 打开继电器
			delay(1000);                           // 延时1s		
			digitalWrite(makerobo_RelayPin, LOW);  // 关闭继电器			
			delay(1000);                           // 延时1s
	}
	return 0;
}

