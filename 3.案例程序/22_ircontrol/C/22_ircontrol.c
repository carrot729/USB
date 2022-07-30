/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：22_ircontrol.c
#  版本：V2.0
#  author: zhulin
#  说明：红外遥控器实验
*****************************************/
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <lirc/lirc_client.h>
#include <time.h>

#define uchar unsigned char

#define makerobo_LedPinRed    0    // RGB—LED,红色管脚定义
#define makerobo_LedPinGreen  1    // RGB—LED,绿色管脚定义
#define makerobo_LedPinBlue   2    // RGB—LED,蓝色管脚定义

uchar rgb_color[3] = {0xff, 0xff, 0xff};  // RGB 颜色配置
uchar rgb_Lv[3]    = {0x00, 0x44, 0xff};  // RGB 亮度配置

// 红外遥控器键值表
char *ir_keymap[21] ={
	" KEY_CHANNELDOWN ",
	" KEY_CHANNEL ",
	" KEY_CHANNELUP ",
	" KEY_PREVIOUS ",
	" KEY_NEXT ",
	" KEY_PLAYPAUSE ",
	" KEY_VOLUMEDOWN ",
	" KEY_VOLUMEUP ",
	" KEY_EQUAL ",
	" KEY_NUMERIC_0 ",
	" BTN_0 ",
	" BTN_1 ",
	" KEY_NUMERIC_1 ",
	" KEY_NUMERIC_2 ",
	" KEY_NUMERIC_3 ",
	" KEY_NUMERIC_4 ",
	" KEY_NUMERIC_5 ",
	" KEY_NUMERIC_6 ",
	" KEY_NUMERIC_7 ",
	" KEY_NUMERIC_8 ",
	" KEY_NUMERIC_9 "};

// RGB-LED 初始化，驱动LED方式为PWM方式
void makerobo_ledInit(void)
{
	softPwmCreate(makerobo_LedPinRed,  0, 100); //初始化 PWM 占空比    
	softPwmCreate(makerobo_LedPinGreen,0, 100); //初始化 PWM 占空比  
	softPwmCreate(makerobo_LedPinBlue, 0, 100); //初始化 PWM 占空比  
}

// RGB-LED 颜色设置
void makerobo_ledColorSet()
{
	softPwmWrite(makerobo_LedPinRed,   rgb_color[0]); // RGB—LED颜色设置
	softPwmWrite(makerobo_LedPinGreen, rgb_color[1]);
	softPwmWrite(makerobo_LedPinBlue,  rgb_color[2]);
}

// 键值查询
int key_scan(char *code){
	int i;
	int num;
	for (i=0; i<21; i++){
		if (strstr(code, ir_keymap[i])){
			num = i;
		}
	}
	return num + 1;
}

// RGB-LED控制
void RGB_control (int i){
	switch(i){
		case 1: rgb_color[0] = rgb_Lv[0]; printf("Makerobo Red OFF\n"); break;   //遥控器的第一行
		case 2: rgb_color[0] = rgb_Lv[1]; printf("Makerobo Light Red\n"); break;
		case 3: rgb_color[0] = rgb_Lv[2]; printf("Makerobo Dark Red\n"); break;
		case 4: rgb_color[1] = rgb_Lv[0]; printf("Makerobo Green OFF\n"); break; // 遥控器的第二行
		case 5: rgb_color[1] = rgb_Lv[1]; printf("Makerobo Light Green\n"); break;
		case 6: rgb_color[1] = rgb_Lv[2]; printf("Makerobo Dark Green\n"); break;
		case 7: rgb_color[2] = rgb_Lv[0]; printf("Makerobo Blue OFF\n"); break;  // 遥控器的第三行
		case 8: rgb_color[2] = rgb_Lv[1]; printf("Makerobo Light Blue\n"); break;
		case 9: rgb_color[2] = rgb_Lv[2]; printf("Makerobo Dark Green\n"); break;
	}
}

// 主程序
int main(void)
{
	struct lirc_config *config;
	int makerobo_buttonTimer = millis();  //获取系统时间
	char *makerobo_code;
	// 初始化连接失败时，将消息打印到屏幕	
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1; 
	}
	// 红外遥控器初始化错误，将消息打印在屏幕
	if(lirc_init("lirc",1)==-1)
		exit(EXIT_FAILURE);

	makerobo_ledInit();       // RGB-LED 初始化，驱动LED方式为PWM方式
	makerobo_ledColorSet();   // RGB-LED 颜色设置
	
	if(lirc_readconfig(NULL,&config,NULL)==0)  // 判断是否有键按下
	{
		while(lirc_nextcode(&makerobo_code)==0)
		{
			if(makerobo_code==NULL) continue;{    
				if (millis() - makerobo_buttonTimer  > 400){ // 判断按键按下的时间
					RGB_control(key_scan(makerobo_code));        //调用RGB-LED控制函数
					makerobo_ledColorSet(makerobo_code);    // 调用颜色控制函数
				}
			}
			free(makerobo_code);
		}
		lirc_freeconfig(config);  // 释放config结构
	}
	lirc_deinit();          // 调用lirc销毁函数
	exit(EXIT_SUCCESS);     // 退出红外遥控器监测
	return 0;
}
