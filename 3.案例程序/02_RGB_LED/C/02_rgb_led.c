
/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：02_Rgb_Led.c
#  版本：V2.0
#  author: zhulin
# 说明：RGB-LED传感器实验
*****************************************/
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define uchar unsigned char

#define makerobo_Led_PinRed    0 // 红色LED 管脚
#define makerobo_Led_PinGreen  1 // 绿色LED 管脚
#define makerobo_Led_PinBlue   2 // 蓝色LED 管脚

// LED 初始化
void makerobo_led_Init(void)
{
	softPwmCreate(makerobo_Led_PinRed,  0, 100);
	softPwmCreate(makerobo_Led_PinGreen,0, 100);
	softPwmCreate(makerobo_Led_PinBlue, 0, 100);
}
// LED 颜色设置
void makerobo_led_Color_Set(uchar r_val, uchar g_val, uchar b_val)
{
	softPwmWrite(makerobo_Led_PinRed,   r_val);
	softPwmWrite(makerobo_Led_PinGreen, g_val);
	softPwmWrite(makerobo_Led_PinBlue,  b_val);
}

//-------------主程序-----------------
int main(void)
{
    //初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1; 
	}
	makerobo_led_Init();

	while(1)
	{
		makerobo_led_Color_Set(0xff,0x00,0x00);   // 红色	
		delay(500);                      // 延时500ms
		makerobo_led_Color_Set(0x00,0xff,0x00);   // 绿色
		delay(500);                      // 延时500ms
		makerobo_led_Color_Set(0x00,0x00,0xff);   // 蓝色
		delay(500);

		makerobo_led_Color_Set(0xff,0xff,0x00);   //黄色
		delay(500);                      // 延时500ms
		makerobo_led_Color_Set(0xff,0x00,0xff);   //pick
		delay(500);                      // 延时500ms
		makerobo_led_Color_Set(0xc0,0xff,0x3e);
		delay(500);                     // 延时500ms

		makerobo_led_Color_Set(0x94,0x00,0xd3);
		delay(500);
		makerobo_led_Color_Set(0x76,0xee,0x00);
		delay(500);
		makerobo_led_Color_Set(0x00,0xc5,0xcd);	
		delay(500);

	}

	return 0;
}
