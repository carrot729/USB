#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：06_button.py
#  版本：V2.0
#  author: zhulin
# 说明：轻触按键实验
#####################################################

import RPi.GPIO as GPIO

makerobo_BtnPin = 11 #轻触按键Pin端口
makerobo_Rpin   = 12 #红色LEDPin端口
makerobo_Gpin   = 13 #绿色LEDPin端口

# 初始化GPIO口
def makerobo_setup():
	GPIO.setmode(GPIO.BOARD)       # 采用实际的物理管脚给GPIO口
	GPIO.setwarnings(False)         # 去除GPIO口警告
	GPIO.setup(makerobo_Rpin, GPIO.OUT)     # 设置红色LED管脚为输出模式
	GPIO.setup(makerobo_Gpin, GPIO.OUT)     # 设置绿色LED管脚为输出模式
	GPIO.setup(makerobo_BtnPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)    # 设置BtnPin管脚为输入模式，上拉至高电平(3.3V)
	# 中断函数，当轻触按键按下时，调用detect函数
	GPIO.add_event_detect(makerobo_BtnPin, GPIO.BOTH, callback=makerobo_detect, bouncetime=200)

# 双色LED模块驱动子函数
def double_colorLED(x):
	if x == 0:                    # x为0时，开启红色LED灯
		GPIO.output(makerobo_Rpin, 1)
		GPIO.output(makerobo_Gpin, 0)
	if x == 1:                    # x为1时，开启绿色LED灯
		GPIO.output(makerobo_Rpin, 0)
		GPIO.output(makerobo_Gpin, 1)

# 打印函数，显示出按键按下
def makerobo_Print(x):
	if x == 0:
		print('***************************************') 
		print('* makerobo Raspberry Kit Button Pressed!   *') 
		print('***************************************')
		
# 中断函数，有按键按下时，响应中断函数
def makerobo_detect(chn):
	double_colorLED(GPIO.input(makerobo_BtnPin))     # 调用双色LED驱动函数
	makerobo_Print(GPIO.input(makerobo_BtnPin))       # 打印出GPIO的状态

# 循环函数	
def makerobo_loop():
	while True:
		pass

# 释放资源
def makerobo_destroy():
	GPIO.output(makerobo_Gpin, GPIO.LOW)       # 关闭绿色LED
	GPIO.output(makerobo_Rpin, GPIO.LOW)       # 关闭红色LED
	GPIO.cleanup()                             # 释放资源

# 程序入口
if __name__ == '__main__':   
	makerobo_setup()           # 初始化GPIO口
	try:
		makerobo_loop()        # 循环函数
	except KeyboardInterrupt:  # 当按下Ctrl+C时，将执行destroy()子程序。
		makerobo_destroy()     # 资源释放

