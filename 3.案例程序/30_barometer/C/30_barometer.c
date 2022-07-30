/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：30_barometer.c
#  版本：V2.0
#  author: zhulin
#  说明：BMP180气压传感器实验
*****************************************/
#include "bmp180.h"    // 载入bmp180库
#include <unistd.h>
#include <stdio.h>

// 主函数
int main(int argc, char **argv){
	char *makerobo_i2c_device = "/dev/i2c-1";     // 总线类型
	int makerobo_address = 0x77;                  // 总线地址

	void *makerobo_bmp = bmp180_init(makerobo_address, makerobo_i2c_device);  // 初始化bmp180总线
     
	 // 如果初始化成功
	if(makerobo_bmp != NULL){
		int makerobo_i;
		for(makerobo_i = 0; makerobo_i < 100; makerobo_i++)
		 {
			float makerobo_t = bmp180_temperature(makerobo_bmp);  // 读取温度值
			long makerobo_p = bmp180_pressure(makerobo_bmp);      // 读取气压值
			float makerobo_alt = bmp180_altitude(makerobo_bmp);   // 读取海拔高度
			printf("Makerobo temperature = %.2f, pressure = %lu, altitude = %.2f\n", makerobo_t, makerobo_p, makerobo_alt); // 打印出该值
			usleep(2 * 1000 * 1000);  // 延时2s
		}
	}
	return 0;
}
