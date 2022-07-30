#/usr/bin/env python3
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：16_analog_hall_switch.py
#  版本：V2.0
#  author: zhulin
# 说明：模拟霍尔传感器实验
#####################################################
import RPi.GPIO as GPIO
import PCF8591 as ADC
import time

# 模块地址设置
def makerobo_setup():
	ADC.setup(0x48) # 设置PCF8591模块地址

# 打印出磁场的信息
def makerobo_Print(x):
	if x == 0:    #  没有磁场
		print ('')
		print ('*************')
		print ('* No Magnet *')
		print ('*************')
		print ('')
	if x == 1:    #  磁场为南
		print ('')
		print ('****************')
		print ('* Magnet North *')
		print ('****************')
		print ('')
	if x == -1:   # 磁场为北
		print ('')
		print ('****************')
		print ('* Magnet South *')
		print ('****************')
		print ('')

# 无限循环
def makerobo_loop():
	makerobo_status = 0 # 给状态变量赋0值
	while True:  # 无限循环
		makerobo_res = ADC.read(0) # 获取AIN0 的模拟量值；
		print ('Current intensity of magnetic field : ', makerobo_res) # 打印出磁场的电流强度值
		if makerobo_res - 133 < 5 and makerobo_res - 133 > -5: # 判断磁场强度范围
			makerobo_tmp = 0    #  没有磁场
		if makerobo_res < 128:  # 磁场强度为北
			makerobo_tmp = -1
		if makerobo_res > 138:  # 磁场强度为南
			makerobo_tmp = 1
		if makerobo_tmp != makerobo_status: # 磁场发生改变
			makerobo_Print(makerobo_tmp) # 调用打印出磁场的信息
			makerobo_status = makerobo_tmp # 把当前状态值设置为比较状态值，避免重复打印；
		time.sleep(0.2) # 延时200ms 

# 程序入口
if __name__ == '__main__':
	makerobo_setup() # 调用初始化程序
	makerobo_loop()  # 调用循环程序

