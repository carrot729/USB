#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：34_temp_monitor.py
#  版本：V2.0
#  author: zhulin
# 说明：综合案例-智能温度测量系统
#####################################################
import RPi.GPIO as  GPIO
import importlib
import time
import sys

# 管脚编号
makerobo_LedR	=	11    # 红色LED灯
makerobo_LedG	=	12    # 绿色LED灯
makerobo_LedB	=	13    # 蓝色LED灯
makerobo_Buzz	=	15    # 蜂鸣器管脚

# 载入其他文件函数
makerobo_joystick	=	importlib.import_module('14_joystick_PS2')   # 载入PS2手柄操作
makerobo_ds18b20	=	importlib.import_module('25_ds18b20')        # 载人DS18b20温度传感器文件
makerobo_beep		=	importlib.import_module('09_active_buzzer')  # 载入有源蜂鸣器文件
makerobo_rgb		=	importlib.import_module('02_rgb_led')        # 载入rgb_led文件

makerobo_joystick.makerobo_setup()   # 初始化PS2手柄
makerobo_ds18b20.makerobo_setup()    # 初始化DS18b20
makerobo_beep.makerobo_setup(makerobo_Buzz)  # 初始化有源蜂鸣器
makerobo_rgb.makerobo_setup(makerobo_LedR, makerobo_LedG, makerobo_LedB) # 初始化RGB—LED模块

makerobo_color = {'Red':0xFF0000, 'Green':0x00FF00, 'Blue':0x0000FF}     # 定义颜色值

# 初始化函数
def makerobo_setup():
	global ds_lowl, ds_highl  # 定义温度值的上下限值
	ds_lowl = 29              # 初始化下限值为29度
	ds_highl = 31             # 初始化上限值为31度

# 计算
def makerobo_edge():
	global ds_lowl, ds_highl
	ds_temp = makerobo_joystick.makerobo_direction()   # 调用PS2操作函数进行方向判断
	if ds_temp == 'Pressed':
		makerobo_destroy()   # 退出系统
		quit()
	if ds_temp == 'up' and ds_lowl < ds_highl-1:   # PS2手柄向上拨动  上限值加1
		ds_highl += 1
	if ds_temp == 'down' and ds_lowl >= -5:        # PS2手柄向下拨动  上限值减1
		ds_highl -= 1
	if ds_temp == 'right' and ds_highl <= 125:     # PS2手柄向右拨动 下限值加1
		ds_lowl += 1
	if ds_temp == 'left' and ds_lowl < ds_highl-1: # PS2手柄向左拨动 下限值减1
		ds_lowl -= 1

# 无限循环函数
def makerobo_loop():
	while True:
		makerobo_edge()       #  调用计算函数
		ds_temp = makerobo_ds18b20.makerobo_read()  # 读取温度值
		print ('Makerobo The lower limit of temperature : ', ds_lowl)    # 打印出下限值
		print ('Makerobo The upper limit of temperature : ', ds_highl)   # 打印出上限值
		print ('Makerobo Current temperature : ', ds_temp)               # 打印读取DS18B20温度值
		if float(ds_temp) < float(ds_lowl):                              # 当实际温度值小于下限值，驱动蜂鸣器发出警报
			makerobo_rgb.set_Color(makerobo_color['Blue'])               # RGB—LED 显示蓝色
			for i in range(0, 3):
				makerobo_beep.makerobo_beep(0.5)                         # 蜂鸣器发出叫声
		if ds_temp >= float(ds_lowl) and ds_temp < float(ds_highl):      # 当实际温度值在上下限值之内，则工作正常
			makerobo_rgb.set_Color(makerobo_color['Green'])              # RGB—LED 显示绿色
		if ds_temp >= float(ds_highl):                                      #  当实际温度值大于上限值
			makerobo_rgb.set_Color(makerobo_color['Red'])                   # RGB—LED 显示红色
			for i in range(0, 3):
				makerobo_beep.makerobo_beep(0.1)                         #  蜂鸣器发出叫声

# 释放资源
def makerobo_destroy():
	makerobo_beep.destroy()                                              # 释放有源蜂鸣器资源
	makerobo_joystick.destroy()                                          # 释放PS2手柄资源
	makerobo_ds18b20.destroy()                                           # 释放DS18b20释放
	makerobo_rgb.destroy()                                               # 释放rgb资源

# 程序入口
if __name__ == "__main__":
	try:
		makerobo_setup()                                                # 调用初始化
		makerobo_loop()                                                 # 调用循环函数
	except KeyboardInterrupt:                                           # 当按下Ctrl+C时，将执行destroy()子程序。
		makerobo_destroy()                                              # 调用释放函数