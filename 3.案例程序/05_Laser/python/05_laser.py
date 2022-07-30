#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：05_laser.py
#  版本：V2.0
#  author: zhulin
# 说明：继电器模块实验
# 注意事项：不要直接用眼睛看激光光束!
#####################################################
import RPi.GPIO as GPIO
import time

makerobo_LaserPin = 11     # 定义激光传感器管脚为Pin11 

# 初始化工作
def makerobo_setup():
	GPIO.setmode(GPIO.BOARD)       # 采用实际的物理管脚给GPIO口
	GPIO.setwarnings(False)         # 去除GPIO口警告
	GPIO.setup(makerobo_LaserPin, GPIO.OUT)   # 设置Pin模式为输出模式
	GPIO.output(makerobo_LaserPin, GPIO.LOW)  # 设置激光传感器管脚为低电平(0V)关闭激光传感器

# 循环函数
def makerobo_loop():
	while True:
		# 打开激光传感器
		GPIO.output(makerobo_LaserPin, GPIO.HIGH)  # 打开激光传感器
		time.sleep(0.5)                       # 延时500mm
		# 关闭激光传感器
		GPIO.output(makerobo_LaserPin, GPIO.LOW) # 关闭激光传感器
		time.sleep(0.5)                     # 延时500mm

# 释放资源
def makerobo_destroy():
	GPIO.output(makerobo_LaserPin, GPIO.LOW)    # 关闭激光传感器
	GPIO.cleanup()                         # 释放资源
	
# 程序入口
if __name__ == '__main__':     
	makerobo_setup()            # 初始化工作
	try:
		makerobo_loop()         # 循环函数
	except KeyboardInterrupt:   # 当按下Ctrl+C时，将执行destroy()子程序。
		makerobo_destroy()      # 释放资源

