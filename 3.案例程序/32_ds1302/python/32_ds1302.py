#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：32_ds1302.py
#  版本：V2.0
#  author: zhulin
# 说明：DS1302实时时钟模块试验
#		ds1302 Module 			   Pi
#			VCC ------------------ 5 V (Must be 5v)
#			GND ------------------ GND
#			CLK ---------------- GPIO23
#			DAT ---------------- GPIO24
#			RST ---------------- GPIO25
#####################################################
from datetime import datetime
from ds1302 import makerobo_DS1302
from sys import version_info
import time

if version_info.major == 2:
	input = raw_input

makerobo_rtc = makerobo_DS1302()   # 实例化ds1302对象

# 初始化程序
def makerobo_setup():
	print ('')
	print ('')
	print (makerobo_rtc.get_datetime())  # 获取时间信息
	print ('')
	print ('')
	ds_a = input( "Makerobo Do you want to setup date and time?(y/n) ")      # 是否更新时间
	if ds_a == 'y' or ds_a == 'Y':                                           # 重新更新时间
		ds_date = input("Input date:(YYYY MM DD) ")                          # 输入年月日
		ds_time = input("Input time:(HH MM SS) ")                            # 输入时分秒
		ds_date = list(map(lambda x: int(x), ds_date.split()))                  # 判断格式        
		ds_time = list(map(lambda x: int(x), ds_time.split()))                  # 判断格式
		print ('')
		print ('')
		makerobo_rtc.set_datetime(datetime(ds_date[0], ds_date[1], ds_date[2], ds_time[0], ds_time[1], ds_time[2])) # 设置时间
		ds_dt = makerobo_rtc.get_datetime()             # 获取当前时间
		print ("You set the date and time to:", ds_dt)  # 打印出当前时间

# 循环函数
def makerobo_loop():
	while True:
		a = makerobo_rtc.get_datetime()  # 获取时间
		print (a)       # 打印出时间
		time.sleep(0.5) # 延时500ms

# 释放资源
def makerobo_destory():
	pass				# 释放资源

# 程序入口
if __name__ == '__main__':		
	makerobo_setup()
	try:
		makerobo_loop()         # 循环函数
	except KeyboardInterrupt:  	# 当按下Ctrl+C时，将执行destroy()子程序。
		makerobo_destory()      # 释放资源