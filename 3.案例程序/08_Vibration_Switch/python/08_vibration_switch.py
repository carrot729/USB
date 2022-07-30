#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：08_vibration_switch.py
#  版本：V2.0
#  author: zhulin
#  说明：振动传感器实验
#####################################################
import RPi.GPIO as GPIO
import time

makerobo_VibratePin = 11 #振动传感器Pin端口
makerobo_Rpin       = 12 #红色LEDPin端口
makerobo_Gpin       = 13 #绿色LEDPin端口

clb_tmp = 0

# 初始化设置
def makerobo_setup():
	GPIO.setmode(GPIO.BOARD)       # 采用实际的物理管脚给GPIO口
	GPIO.setwarnings(False)         # 去除GPIO口警告
	GPIO.setup(makerobo_Rpin, GPIO.OUT)     # 设置红色LED管脚为输出模式
	GPIO.setup(makerobo_Gpin, GPIO.OUT)     # 设置绿色LED管脚为输出模式
	GPIO.setup(makerobo_VibratePin, GPIO.IN, pull_up_down=GPIO.PUD_UP)    # 设置makerobo_VibratePin管脚为输入模式，上拉至高电平(3.3V)

# 双色LED模块驱动子函数
def double_colorLED(x):
	if x == 0:                         # x为0时，开启红色LED灯
		GPIO.output(makerobo_Rpin, 1)
		GPIO.output(makerobo_Gpin, 0)
	if x == 1:                         # x为1时，开启绿色LED灯
		GPIO.output(makerobo_Rpin, 0)
		GPIO.output(makerobo_Gpin, 1)

# 打印信息,打印出振动传感器的状态		
def makerobo_Print(x):
	global clb_tmp
	if x != clb_tmp:
		if x == 0:
			print('******************')
			print('* Makerobo    ON *')
			print('******************')
	
		if x == 1:
			print('*******************')
			print('* OFF  Makerobo   *')
			print('*******************')
		clb_tmp = x

# 循环函数	
def makerobo_loop():
	clb_state = 0
	while True:
		if GPIO.input(makerobo_VibratePin)==1:  # 判断振动传感的值，如果为0有振动产生
			clb_state = clb_state + 1           # 振动传感器状态加1
			if clb_state > 1:                   # 状态发生改变判断
				clb_state = 0                   # 状态值复位
			double_colorLED(clb_state)          # 控制双色LED模块
			makerobo_Print(clb_state)           # 打印出状态
			time.sleep(1)                       # 延时1s

# 释放资源
def makerobo_destroy():
	GPIO.output(makerobo_Rpin, GPIO.HIGH)       # 关闭红色LED
	GPIO.output(makerobo_Gpin, GPIO.HIGH)       # 关闭绿色LED
	GPIO.cleanup()                              # 释放资源

# 程序入口
if __name__ == '__main__':    
	makerobo_setup()           # 初始化设置
	try:
		makerobo_loop()        # 循环函数
	except KeyboardInterrupt:  # 当按下Ctrl+C时，将执行destroy()子程序。
		makerobo_destroy()     # 释放资源

