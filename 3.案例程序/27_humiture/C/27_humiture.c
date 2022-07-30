/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：27_humiture.c
#  版本：V2.0
#  author: zhulin
#  说明：湿度传感器检测实验
*****************************************/
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXTIMINGS 85              // 等待最大时间
#define makerobo_DHTPIN 0         // DHT11 传感器Pin管脚

int makerobo_dht11_dat[5] = {0,0,0,0,0};  // dht11 数据数组

// 读取dht11温湿度传感器数据函数
void makerobo_read_dht11_dat()
{
	uint8_t dht_laststate = HIGH;  // 等待高电平
	uint8_t makerobo_counter = 0; // 定义计数值
	uint8_t j = 0, i;
	float f;  // 华氏温度

    // 重新给dht11数据数值赋初值
	makerobo_dht11_dat[0] = makerobo_dht11_dat[1] = makerobo_dht11_dat[2] = makerobo_dht11_dat[3] = makerobo_dht11_dat[4] = 0;

	// DHT11 管脚下拉18ms
	pinMode(makerobo_DHTPIN, OUTPUT);     // 设置makerobo_DHTPIN 为输出模式
	digitalWrite(makerobo_DHTPIN, LOW);   // DHT 11 传感器 输出为低电平
	delay(18);                            // 延时18ms
	// 然后上拉40微秒
	digitalWrite(makerobo_DHTPIN, HIGH); // DHT 11 传感器 输出为高电平
	delayMicroseconds(40);               // 延时40ms

	// 准备读取DHT11传感器
	pinMode(makerobo_DHTPIN, INPUT);   // 设置DHT11 传感器为输入模式

	// 检测并读取数据  
	// 等待 85 微秒的高电平后进行数据接收
	for ( i=0; i< MAXTIMINGS; i++) 
	{
		makerobo_counter = 0;         // 计数值复位
		while (digitalRead(makerobo_DHTPIN) == dht_laststate) {  //等待高电平结束
			makerobo_counter++;        // 计数值累加
			delayMicroseconds(1);      // 延时1ms
			if (makerobo_counter == 255) {      // 如果等待的时间过长，则退出
				break;
			}
		}       
		dht_laststate = digitalRead(makerobo_DHTPIN); // 重新读取DTH11电平信号
		if (makerobo_counter == 255) break; // 如果等待的时间过长，则退出

		// 忽略前三个数据
		if ((i >= 4) && (i%2 == 0)) {
			// 将每位放入存储字节中
			makerobo_dht11_dat[j/8] <<= 1;
			if (makerobo_counter > 16)
				makerobo_dht11_dat[j/8] |= 1;
			j++;
		}
	}
	// 检查我们读取40位(8bit x 5) +校验校验和在最后一个字节打印出来
	// 如果数据是正常的！
	if ((j >= 40) && 
			(makerobo_dht11_dat[4] == ((makerobo_dht11_dat[0] + makerobo_dht11_dat[1] + makerobo_dht11_dat[2] + makerobo_dht11_dat[3]) & 0xFF)) ) {
		f = makerobo_dht11_dat[2] * 9. / 5. + 32; // 计算温度值
		// 打印出温度值和湿度值
		printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n", 
				makerobo_dht11_dat[0], makerobo_dht11_dat[1], makerobo_dht11_dat[2], makerobo_dht11_dat[3], f);
	}
}

// 主函数
int main (void)
{
    // 打印开始检查信息
	printf (" Makerobo Raspberry Pi wiringPi DHT11 Temperature test program\n") ;
	
	// 初始化连接失败时，将退出
	if (wiringPiSetup () == -1)
		exit (1) ;
    
	// 无限循环
	while (1) 
	{
		makerobo_read_dht11_dat(); // 读取dht11温湿度传感器数据函数
		delay(1000);               //等待1秒刷新
	}
	return 0 ;
}
