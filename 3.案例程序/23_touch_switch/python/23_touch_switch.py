#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：23_touch_switch.py
#  版本：V2.0
#  author: zhulin
# 说明：触摸开关传感器模块
#####################################################
import RPi.GPIO as GPIO

makerobo_TouchPin = 11   # 触摸传感器管脚PIN
makerobo_Rpin     = 12   # 红色LED管脚PIN
makerobo_Gpin     = 13   # 绿色LED管脚PIN

makerobo_tmp = 0    #是否有触摸判断 

# GPIO口定义
def makerobo_setup():
	GPIO.setmode(GPIO.BOARD)       # 采用实际的物理管脚给GPIO口
	GPIO.setwarnings(False)        # 忽略GPIO操作注意警告
	GPIO.setup(makerobo_Rpin, GPIO.OUT)     # 设置红色LED管脚为输出模式
	GPIO.setup(makerobo_Gpin, GPIO.OUT)     # 设置绿色LED管脚为输出模式
	GPIO.setup(makerobo_TouchPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)    # 设置makerobo_TouchPin管脚为输入模式，上拉至高电平(3.3V)

# 双色LED模块驱动子函数
def double_colorLED(x):
	if x == 1:          # x为1时，开启红色LED灯
		GPIO.output(makerobo_Rpin, 1)
		GPIO.output(makerobo_Gpin, 0)
	if x == 0:         # x为0时，开启绿色LED灯
		GPIO.output(makerobo_Rpin, 0)
		GPIO.output(makerobo_Gpin, 1)	

# 打印函数，显示出是否有触摸
def makerobo_Print(x):
	global makerobo_tmp
	if x != makerobo_tmp:
		if x == 1:    #  发生触摸
			print ('*************************')
			print ('* Makerobo Have a touch *')
			print ('*************************')
	
		if x == 0:    # 没有发生触摸
			print ('*****************************')
			print ('* Makerobo No touch occurred*')
			print ('*****************************')
		makerobo_tmp = x  # 触摸状态值保存和下次做比较，避免重复打印

# 循环函数
def makerobo_loop():
	while True:  # 无限循环
		double_colorLED(GPIO.input(makerobo_TouchPin)) # 调用双色LED模块驱动子函数
		makerobo_Print(GPIO.input(makerobo_TouchPin))  # 调用打印函数，显示出是否有触摸

# 资源释放函数
def destroy():
	GPIO.output(makerobo_Gpin, GPIO.LOW)       # 关闭绿色LED
	GPIO.output(makerobo_Rpin, GPIO.LOW)       # 关闭红色LED
	GPIO.cleanup()                     # 释放资源

# 程序入口
if __name__ == '__main__':    
	makerobo_setup()  # GPIO口定义
	try:
		makerobo_loop() # 循环函数
	except KeyboardInterrupt:   # 当按下Ctrl+C时，将执行destroy()子程序。
		destroy() # 资源释放函数

