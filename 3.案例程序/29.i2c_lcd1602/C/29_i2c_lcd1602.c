/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：29_i2c_lcd1602.c
#  版本：V2.0
#  author: zhulin
#  说明：IIC LCD1602液晶显示器模块实验
*****************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>

int makerobo_LCDAddr = 0x27;   // LCD 1602 液晶IIC地址 0x27或者0x3F
int makerobo_BLEN = 1;         // 写入标志位
int makerobo_fd;

// IIC LCD1602 液晶模块写入字
void makerobo_write_word(int data){
	int temp = data;
	if ( makerobo_BLEN == 1 )
		temp |= 0x08;
	else
		temp &= 0xF7;
	wiringPiI2CWrite(makerobo_fd, temp);   //设置IIC LCD1602 液晶模块地址
}
// IIC LCD1602 发送命令
void makerobo_send_command(int comm){
	int lcd_buf;
	// 首先发送 bit7-4 位
	lcd_buf = comm & 0xF0;
	lcd_buf |= 0x04;			// RS = 0, RW = 0, EN = 1
	makerobo_write_word(lcd_buf);
	delay(2);
	lcd_buf &= 0xFB;			// Make EN = 0
	makerobo_write_word(lcd_buf);

	// 其次发送 bit3-0 位
	lcd_buf = (comm & 0x0F) << 4;
	lcd_buf |= 0x04;			// RS = 0, RW = 0, EN = 1
	makerobo_write_word(lcd_buf);
	delay(2);
	lcd_buf &= 0xFB;			// Make EN = 0
	makerobo_write_word(lcd_buf);
}
// IIC LCD1602 发送数据
void makerobo_send_data(int data){
	int lcd_buf;
	// 首先发送 bit7-4 位
	lcd_buf = data & 0xF0;
	lcd_buf |= 0x05;			// RS = 1, RW = 0, EN = 1
	makerobo_write_word(lcd_buf);
	delay(2);
	lcd_buf &= 0xFB;			// Make EN = 0
	makerobo_write_word(lcd_buf);

	// 其次发送 bit3-0 位
	lcd_buf = (data & 0x0F) << 4;
	lcd_buf |= 0x05;			// RS = 1, RW = 0, EN = 1
	makerobo_write_word(lcd_buf);
	delay(2);
	lcd_buf &= 0xFB;			// Make EN = 0
	makerobo_write_word(lcd_buf);
}
// IIC LCD1602 初始化
void makerobo_init(){
	makerobo_send_command(0x33);	// 必须先初始化到8线模式吗
	delay(5);
	makerobo_send_command(0x32);	// 然后初始化为4行模式
	delay(5);
	makerobo_send_command(0x28);	// 2 行 & 5*7 点位
	delay(5);
	makerobo_send_command(0x0C);	// 启用无光标显示
	delay(5);
	makerobo_send_command(0x01);	// 清除显示
	wiringPiI2CWrite(makerobo_fd, 0x08);
}
// LCD 1602 清空显示函数
void makerobo_clear()
{
	makerobo_send_command(0x01);	// 清除显示
}
// LCD 1602 显示函数
void makerobo_write(int lcd_x, int lcd_y, char data[]){
	int lcd_addr, lcd_i;
	int lcd_tmp;
	// 选择行与列
	if (lcd_x < 0)  lcd_x = 0;
	if (lcd_x > 15) lcd_x = 15;
	if (lcd_y < 0)  lcd_y = 0;
	if (lcd_y > 1)  lcd_y = 1;

	// 移动光标
	lcd_addr = 0x80 + 0x40 * lcd_y + lcd_x;
	
	makerobo_send_command(lcd_addr); //发送地址
	
	lcd_tmp = strlen(data);         // 获取字符串长度
	for (lcd_i = 0; lcd_i < lcd_tmp; lcd_i++){  // 依次发送
		makerobo_send_data(data[lcd_i]);    // 逐一显示
	}
}
// 主函数
int main(){
	makerobo_fd = wiringPiI2CSetup(makerobo_LCDAddr);  //初始化地址
	makerobo_init();                                   //初始化显示屏
	while(1)
	{
		makerobo_write(0, 0, "Hello!");                    //第一行显示
		makerobo_write(0, 1, "Makerobo kit");              //第二行显示
		delay(2000);                                      // 延时2s
		makerobo_clear();                                 // 清空显示
	}
	return 0;
}
