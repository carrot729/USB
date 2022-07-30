/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：31_mpu6050.c
#  版本：V2.0
#  author: zhulin
#  说明：MPU6050 陀螺仪加速度传感器
*****************************************/
#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>

#define makerobot_Device_Address 0x68	 // MPU6050的设备地址/标识符

// MPU6050 寄存器地址
#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47

int fd;

// MPU6050 初始化
void MPU6050_Init()
{	
	wiringPiI2CWriteReg8 (fd, SMPLRT_DIV, 0x07);	// 写入抽样速率寄存器
	wiringPiI2CWriteReg8 (fd, PWR_MGMT_1, 0x01);	// 写入电源管理寄存器
	wiringPiI2CWriteReg8 (fd, CONFIG, 0);		      // 写入配置寄存器
	wiringPiI2CWriteReg8 (fd, GYRO_CONFIG, 24);	  // 写入陀螺配置寄存器
	wiringPiI2CWriteReg8 (fd, INT_ENABLE, 0x01);	// 写中断使能寄存器
} 

// MPU6050 读取寄存器
short read_raw_data(int addr)
{
	short high_byte,low_byte,value;
	high_byte = wiringPiI2CReadReg8(fd, addr);
	low_byte = wiringPiI2CReadReg8(fd, addr+1);
	value = (high_byte << 8) | low_byte;
	return value;
}

//  延时毫秒数
void ms_delay(int val){
	int i,j;
	for(i=0;i<=val;i++)
		for(j=0;j<1200;j++);
}

// 主函数
int main(){
	
	float Acc_x,Acc_y,Acc_z;
	float Gyro_x,Gyro_y,Gyro_z;
	float makerobot_Ax=0, makerobot_Ay=0, makerobot_Az=0;
	float makerobot_Gx=0, makerobot_Gy=0, makerobot_Gz=0;
	fd = wiringPiI2CSetup(makerobot_Device_Address);   // 使用设备地址初始化I2C
	MPU6050_Init();		                       // 初始化MPU6050
	
	while(1)
	{
		// 从MPU6050中读取加速度计和陀螺仪的原始值
		Acc_x = read_raw_data(ACCEL_XOUT_H);
		Acc_y = read_raw_data(ACCEL_YOUT_H);
		Acc_z = read_raw_data(ACCEL_ZOUT_H);
		
		Gyro_x = read_raw_data(GYRO_XOUT_H);
		Gyro_y = read_raw_data(GYRO_YOUT_H);
		Gyro_z = read_raw_data(GYRO_ZOUT_H);
		
		// 原始值除以灵敏度尺度因子
		makerobot_Ax = Acc_x/16384.0;
		makerobot_Ay = Acc_y/16384.0;
		makerobot_Az = Acc_z/16384.0;
		
		makerobot_Gx = Gyro_x/131;
		makerobot_Gy = Gyro_y/131;
		makerobot_Gz = Gyro_z/131;
		// 打印加速度陀螺仪数据
		printf("\n Gx=%.3f °/s\tGy=%.3f °/s\tGz=%.3f °/s\tAx=%.3f g\tAy=%.3f g\tAz=%.3f g\n",makerobot_Gx,makerobot_Gy,makerobot_Gz,makerobot_Ax,makerobot_Ay,makerobot_Az);
		delay(500);
	}
	return 0;
}