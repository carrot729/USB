#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：29_i2c_lcd1602.py
#  版本：V2.0
#  author: zhulin
#  说明：IIC LCD1602液晶显示器模块实验
#####################################################
import LCD1602    # LCD1602液晶显示屏库
import time

# 初始化LCD1602液晶模块
def makerobo_setup():
	LCD1602.makerobo_init(0x27, 1)	# 初始化(设备地址, 背光设置)
	LCD1602.makerobo_write(0, 0, 'Hello!!!')     # 显示第一行信息
	LCD1602.makerobo_write(0, 1, 'Makerobo kit') # 显示第二行信息
	time.sleep(2)                       # 延时2S

# 循环函数
def makerobo_loop():
	makerobo_space = '                '  # 空显信息
	makerobo_greetings = 'Thank you for using the makerobo raspberry pi kit! ^_^' # 显示提示信息
	makerobo_greetings = makerobo_space + makerobo_greetings # 显示信息拼接
	# 无线循环
	while True:  
		makerobo_tmp = makerobo_greetings                    # 获取到显示信息
		for i in range(0, len(makerobo_greetings)):          # 逐一显示
			LCD1602.makerobo_write(0, 0, makerobo_tmp)       # 逐个显示
			makerobo_tmp = makerobo_tmp[1:]
			time.sleep(0.8)                                  # 延时800ms
			LCD1602.makerobo_clear()                         # 清除显示

# 释放资源
def destroy():
	pass	

# 程序入口
if __name__ == "__main__":
	try:
		makerobo_setup()      # 初始化信息
		makerobo_loop()       # 循环显示信息
	except KeyboardInterrupt: # 当按下Ctrl+C时，将执行destroy()子程序。
		destroy()             # 释放资源
