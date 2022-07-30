/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：34_temp_monitor.c
#  版本：V2.0
#  author: zhulin
#  说明：综合案例-智能温度测量系统
*****************************************/
#include <wiringPi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcf8591.h>

#define     makerobo_LedRed		0  // 红色LED模块管脚
#define     makerobo_LedGreen	1  // 绿色LED模块管脚
#define     makerobo_LedBlue	2  // 蓝色LED模块管脚
 
#define     makerobo_PCF	   120  // 基础管脚120    
#define		makerobo_Beep		3   // 蜂鸣器管脚
#define		makerobo_BUFSIZE	128 // 缓存区大小

typedef unsigned char uchar;
typedef unsigned int  uint;

static int  makerobo_sys_state = 1; //  ps2手柄状态,退出系统标志位

// pcf8591 模块 AIN0-AIN2 连接了PS2 操作杆
static int makerobo_AIN0 = makerobo_PCF + 0;  // Y 方向
static int makerobo_AIN1 = makerobo_PCF + 1;  // X 方向
static int makerobo_AIN2 = makerobo_PCF + 2;  // SW 按下

//  蜂鸣器初始化
void makerobo_beepInit(void)
{
	pinMode(makerobo_Beep, OUTPUT);	     // 蜂鸣器设置为输出模式
	digitalWrite(makerobo_Beep, HIGH);	// 蜂鸣器发出高电平停止鸣叫

}
// 蜂鸣器鸣叫
void makerobo_beep_on(void)
{
	digitalWrite(makerobo_Beep, LOW);   // 蜂鸣器发出低电平鸣叫
}
// 关闭蜂鸣器鸣叫
void makerobo_beep_off(void)
{	
	digitalWrite(makerobo_Beep, HIGH);	// 蜂鸣器发出高电平停止鸣叫
}
// 蜂鸣器交替鸣叫声发出报警声
void makerobo_beepCtrl(int t)
{
	                                 // 蜂鸣器发出低电平鸣叫
	makerobo_beep_on();
	delay(t);                       // 延时函数，延时时间通过形参指定
	makerobo_beep_off();            // 关闭蜂鸣器鸣叫
	delay(t);                       // 延时函数，延时时间通过形参指定
}

// 读取DS18B20 的温度值
float makerobo_tempRead(void)
{
	float ds_temp;        // 温度值
	int ds_i, ds_j;       // 零时变量
    int ds_fd;            // 读取文件句柄
	int ds_ret;           // 获取温度值

	char ds_buf[makerobo_BUFSIZE];    // 寄存器缓冲区
	char ds_tempBuf[5];      // 温度缓冲区
	// 28-XXXXX为DS18B20 地址，每一个DS18B20 地址都不一样,注意替换
	ds_fd = open("/sys/bus/w1/devices/28-3c01a81659d8/w1_slave", O_RDONLY);  // 打开DS18B20 温度传感器文件获取温度
    // 设备读取错误
	if(-1 == ds_fd){
		perror("Makerobo open device file error"); // 打印出错误信息
		return 1;
	}
    // 无限循环
	while(1){
		ds_ret = read(ds_fd, ds_buf, makerobo_BUFSIZE); // 读取温度信息值
		if(0 == ds_ret){     // 读取的温度信息不为空
			break;	
		}
		// 读取的温度信息不为错误
		if(-1 == ds_ret){
			if(errno == EINTR){
				continue;	
			}
			perror("read()");  // 弹出错误信息
			close(ds_fd);     // 关闭温度传感器
			return 1;
		}
	}
     // 获取到实际的温度值
	for(ds_i=0;ds_i<sizeof(ds_buf);ds_i++){
		if(ds_buf[ds_i] == 't'){
			for(ds_j=0;ds_j<sizeof(ds_tempBuf);ds_j++){
				ds_tempBuf[ds_j] = ds_buf[ds_i+2+ds_j]; 	
			}
		}	
	}
	ds_temp = (float)atoi(ds_tempBuf) / 1000;  //转换为带浮点型的温度数据
	close(ds_fd);    // 关闭温度传感器
	return ds_temp; // 返回温度信息
}

// 初始化RGB-led 模块
void makerobo_ledInit(void)
{
	pinMode(makerobo_LedRed,   OUTPUT);	   // 设置红色LED为输出模式
	pinMode(makerobo_LedGreen, OUTPUT);	   // 设置绿色LED为输出模式
	pinMode(makerobo_LedBlue,  OUTPUT);	   // 设置蓝色LED为输出模式
}

// RGB—LED 控制
void makerobo_ledCtrl(int n, int state)
{
	digitalWrite(n, state);
}
// 发生中断退出
void makerobo_joystickquit(void)
{
	makerobo_sys_state = 0;                 // PS2手柄状态为0
	printf("Makerobo interrupt occur !\n"); // 发生中断
}

// 获取PS2手柄操作的状态
uchar js_get_joyStick_state(void)
{
	uchar js_tmp = 0;                            // ps2手柄状态
	uchar js_xVal = 0, js_yVal = 0, js_zVal = 0; // X方向的值，Y方向的值和sw按下值
	
	js_xVal = analogRead(makerobo_AIN1);  // 获取x方向的值

	if(js_xVal >= 250){    // left
		js_tmp = 2;
	}
	if(js_xVal <= 5){     // right
		js_tmp = 1;
	}

	js_yVal = analogRead(makerobo_AIN0);  // 获取Y方向的值
	if(js_yVal >= 250){    // down
		js_tmp = 4;
	}
	if(js_yVal <= 5){      // up
		js_tmp = 3;
	}
	
	js_zVal = analogRead(makerobo_AIN2); // 获取SW按下的值
	if(js_zVal <= 5){     // button 按下
		js_tmp = 5;
	}
    // home位置
	if(js_xVal-127<30 && js_xVal-127>-30 && js_yVal-127<30 && js_yVal-127>-30 && js_zVal>127){
		js_tmp = 0;  // home位置
	}
	// printf("x = %d, y = %d, z = %d",js_xVal,js_yVal,js_zVal);  // 打印出X、Y Z值
	return js_tmp;   // 返回PS2手柄状态
}

// 主函数
int main(void)
{
	int i;
	uchar makerobo_joyStick = 0;          // ps2手柄状态信息
	float ds_temp;                        // 读取温度值
	uchar ds_low = 26, ds_high = 30;      // 温度值的下限值和上限值

   // 初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1; 
	}
	
	pcf8591Setup(makerobo_PCF, 0x48);      // pcf8591 模块初始化
	makerobo_ledInit();                    // RGB-LED 初始化
	makerobo_beepInit();                   // 蜂鸣器初始化
	
	printf("Makerobo System is running...\n"); // 系统开始运行

	while(1){
		flag:
		makerobo_joyStick = js_get_joyStick_state(); // 获取PS2手柄的状态值
        // PS2手柄操作对温度的上下限值进行操作
		switch(makerobo_joyStick){
			case 1 : --ds_low;  break;            // 下限值减
			case 2 : ++ds_low;  break;            // 下限值加
			case 3 : ++ds_high; break;            // 上限值加
			case 4 : --ds_high; break;            // 上限值减
			case 5 : makerobo_joystickquit(); break;       // 按下中间按键退出温度调节 
			default: break;
		}
		// 当下线值大于上限值弹出错误
		if(ds_low >= ds_high){
			printf("Makerobo Error, lower limit should be less than upper limit\n");  // 不允许下限值大于上限值
			goto flag;
		}

		printf("The lower limit of temperature : %d\n", ds_low);      // 打印出下限值
		printf("The upper limit of temperature : %d\n", ds_high);     // 打印出上限值
		
		ds_temp = makerobo_tempRead();         // 获取DS18B20设备温度值
		
		printf("Makerobo Current temperature : %0.3f\n", ds_temp); // 打印出DS18B20设备温度值 
		
		if(ds_temp < ds_low){         //当实际温度值小于下限值，驱动蜂鸣器发出警报
			makerobo_ledCtrl(makerobo_LedBlue,  LOW);     // 打开蓝色灯
			makerobo_ledCtrl(makerobo_LedRed,   HIGH);    // 关闭红色灯
			makerobo_ledCtrl(makerobo_LedGreen, LOW);     // 打开绿色灯
			for(i = 0;i < 3; i++){
				makerobo_beepCtrl(400); // 蜂鸣器鸣叫
			}
		}
		if(ds_temp >= ds_low && ds_temp < ds_high){  // 当实际温度值在上下限值之内，则工作正常
			makerobo_ledCtrl(makerobo_LedBlue,  HIGH);        // 关闭蓝色灯
			makerobo_ledCtrl(makerobo_LedRed,   HIGH);        // 关闭红色灯
			makerobo_ledCtrl(makerobo_LedGreen, LOW);         // 启动绿色灯
		}
		if(ds_temp >= ds_high){                     // 当实际温度值大于上限值
			makerobo_ledCtrl(makerobo_LedBlue,  HIGH);       // 关闭蓝色灯
			makerobo_ledCtrl(makerobo_LedRed,   LOW);        // 打开红色灯
			makerobo_ledCtrl(makerobo_LedGreen, HIGH);       // 关闭绿色灯
			for(i = 0;i < 3; i++){
				makerobo_beepCtrl(80);
			}
		}
        // 退出系统标志位
		if(makerobo_sys_state == 0){
			makerobo_ledCtrl(makerobo_LedRed, HIGH);     // 关闭红色LED灯
			makerobo_ledCtrl(makerobo_LedGreen, HIGH);   // 关闭绿色LED灯
			makerobo_ledCtrl(makerobo_LedBlue, HIGH);    // 关闭蓝色LED灯
			makerobo_beep_off();    // 关闭LED灯
			printf("Makerobo SyStem will be off...\n");  // 关闭系统
			break;
		}
	}
	return 0;
}
