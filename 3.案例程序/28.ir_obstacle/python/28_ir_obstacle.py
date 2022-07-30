#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：28_ir_obstacle.py
#  版本：V2.0
#  author: zhulin
#  说明：红外避障传感器模块
#####################################################
import RPi.GPIO as GPIO
import time

makerobo_ObstaclePin = 11       # 红外避障传感器管脚PIN

def makerobo_setup():
	GPIO.setmode(GPIO.BOARD)       # 采用实际的物理管脚给GPIO口
	GPIO.setup(makerobo_ObstaclePin, GPIO.IN, pull_up_down=GPIO.PUD_UP) # 设置makerobo_ObstaclePin管脚为输入模式，上拉至高电平(3.3V)

# 无限循环
def makerobo_loop():
	while True:
		if (0 == GPIO.input(makerobo_ObstaclePin)):  # 检测到障碍物
			print ("Makerobo Detected Barrier!")     # 打印出障碍物信息
			time.sleep(0.2)                          # 延时200ms

			 
# 释放资源
def destroy():
	GPIO.cleanup()                     # 释放资源函数

# 程序入口
if __name__ == '__main__':     
	makerobo_setup()           # 初始化设置
	try:
		makerobo_loop()        # 无限循环
	except KeyboardInterrupt:  # 当按下Ctrl+C时，将执行destroy()子程序。
		destroy()              # 调用释放资源

