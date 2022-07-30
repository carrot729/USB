#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：10_reed_switch.py
#  版本：V2.0
#  author: zhulin
# 说明：干簧管传感器实验
#####################################################
import RPi.GPIO as GPIO

makerobo_ReedPin = 11  #干簧管传感器Pin端口
makerobo_Rpin    = 12  #红色LEDPin端口
makerobo_Gpin    = 13  #绿色LEDPin端口

# GPIO口定义
def makerobo_setup():
	GPIO.setmode(GPIO.BOARD)       # 采用实际的物理管脚给GPIO口
	GPIO.setwarnings(False)                       # 关闭GPIO警告提示
	GPIO.setup(makerobo_Rpin, GPIO.OUT)     # 设置红色LED管脚为输出模式
	GPIO.setup(makerobo_Gpin, GPIO.OUT)     # 设置绿色LED管脚为输出模式
	GPIO.setup(makerobo_ReedPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)    # 设置makerobo_ReedPin管脚为输入模式，上拉至高电平(3.3V)
	# 中断函数，当有磁性物质靠近时，调用makerobo_detect函数
	GPIO.add_event_detect(makerobo_ReedPin, GPIO.BOTH, callback=makerobo_detect, bouncetime=10)

# 双色LED模块驱动子函数
def double_colorLED(x):
	if x == 0:              # x为0时，开启红色LED灯
		GPIO.output(makerobo_Rpin, 1)
		GPIO.output(makerobo_Gpin, 0)
	if x == 1:             # x为1时，开启绿色LED灯
		GPIO.output(makerobo_Rpin, 0)
		GPIO.output(makerobo_Gpin, 1)

# 打印函数，显示出检测到磁性物质
def makerobo_Print(x):
	if x == 0:
		print('**********************************************')
		print('***** makerobo Detected Magnetic Material!   *')
		print('**********************************************')

# 中断函数，检测到磁性物质时，响应中断函数		
def makerobo_detect(chn):
	double_colorLED(GPIO.input(makerobo_ReedPin))      # 调用双色LED驱动函数
	makerobo_Print(GPIO.input(makerobo_ReedPin))       # 打印出检测到磁性物质！

# 循环函数
def loop():
	while True:
		pass

def destroy():
	GPIO.output(makerobo_Rpin, GPIO.HIGH)       # 关闭红色LED
	GPIO.output(makerobo_Gpin, GPIO.HIGH)       # 关闭绿色LED
	GPIO.cleanup()                     # 释放资源

# 程序入口
if __name__ == '__main__':
	makerobo_setup() # GPIO 初始化
	try:
		loop()
	except KeyboardInterrupt:  # 当按下Ctrl+C时，将执行destroy()子程序。
		destroy()
