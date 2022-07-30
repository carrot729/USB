#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：04_relay.py
#  版本：V2.0
#  author: zhulin
# 说明：继电器模块实验
#---------------------------------------
import RPi.GPIO as GPIO
import time

makerobo_RelayPin = 11    # 定义继电器管脚为Pin11

# 初始化工作
def makerobo_setup():
    GPIO.setmode(GPIO.BOARD)        # 采用实际的物理管脚给GPIO口
    GPIO.setwarning(False)          # 去除GPIO警告
    GPIO.setup(makerobo_RelayPin, GPIO.OUT)  # 设置Pin模式为输出模式
    GPIO.output(makerobo_RelayPin, GPIO.LOW) # 关闭继电器	


# 循环函数
def makerobo_loop():
	while True:
		# 继电器打开
		GPIO.output(makerobo_RelayPin, GPIO.HIGH)
		time.sleep(0.5)                      # 延时500ms
		# 继电器关闭
		GPIO.output(makerobo_RelayPin, GPIO.LOW)
		time.sleep(0.5)                      # 延时500ms

# 释放资源
def makerobo_destroy():
	GPIO.output(makerobo_RelayPin, GPIO.LOW) # 关闭继电器	
	GPIO.cleanup()                      # 释放资源

# 程序入口
if __name__ == '__main__':
	makerobo_setup()           #  初始化
	try:
		makerobo_loop()        #  调用循环函数
	except KeyboardInterrupt:  #  当按下Ctrl+C时，将执行destroy()子程序。
		makerobo_destroy()     #  释放资源
