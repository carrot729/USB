/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：01_Dule_color_LED.c
#  版本：V2.0
#  author: zhulin
# 说明：双色LED实验
*****************************************/
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define uchar unsigned char

#define makerobo_Led_PinRed    0 // 红色LED 管脚
#define makerobo_Led_PinGreen  1 // 绿色LED 管脚

// LED 初始化
void makerobo_led_Init(void)
{
	softPwmCreate(makerobo_Led_PinRed,  0, 100);
	softPwmCreate(makerobo_Led_PinGreen,0, 100);
}
// 设置LED 亮度PWM调节范围是0x00-0xff
void makerobo_led_ColorSet(uchar r_val, uchar g_val)
{
	softPwmWrite(makerobo_Led_PinRed,   r_val);
	softPwmWrite(makerobo_Led_PinGreen, g_val);
}
//-------------主程序-----------------
int main(void)
{
    //初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}

	makerobo_led_Init(); // LED 初始化

	while(1){
		makerobo_led_ColorSet(0xff,0x00);   //红色	
		delay(500);                         //延时500ms
		makerobo_led_ColorSet(0x00,0xff);   //绿色
		delay(500);
		makerobo_led_ColorSet(0xff,0x45);	
		delay(500);
		makerobo_led_ColorSet(0xff,0xff);	
		delay(500);
		makerobo_led_ColorSet(0x7c,0xfc);	
		delay(500);
	}
	return 0;
}
