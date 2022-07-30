/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：25_ds18b20.c
#  版本：V2.0
#  author: zhulin
#  说明：DS18B20温度传感器实验
*****************************************/
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// 主程序
int main (void) {
 DIR *dir;
 struct dirent *dirent;
 char dev[16];      // 设备ID号
 char devPath[128]; // 设备路径
 char buf[256];     // 设备数据
 char tmpData[6];   // Temp C * 1000 温度数据
 char path[] = "/sys/bus/w1/devices";  // 设备路径位置
 ssize_t numRead;
 
dir = opendir (path); // 访问设备路径
       if (dir != NULL)  // 打开设备不为空
       {
              while ((dirent = readdir (dir)))

              // 单总线设备地址是从28-开始的
              if (dirent->d_type == DT_LNK && 
              strstr(dirent->d_name, "28-") != NULL) { 
              strcpy(dev, dirent->d_name);
              printf("\nDevice: %s\n", dev); // 寻找到设备路径打印出来
              }
              (void) closedir (dir); // 关闭文件操作
       }
       else
       {     // 无法打开目录
              perror ("Couldn't open the w1 devices directory"); 
              return 1;
       }

       // 获取到了具体的DS18B20地址
       sprintf(devPath, "%s/%s/w1_slave", path, dev);
       //连续读取临时变量
       //打开设备的文件触发新的读取
       while(1) 
       {
              int fd = open(devPath, O_RDONLY);  // 打开设备
              if(fd == -1) // 打开失败
              {
                     perror ("Couldn't open the w1 device.");  // 打印失败信息
                     return 1;   
              }
              while((numRead = read(fd, buf, 256)) > 0)    // 读取成功
              {
                     strncpy(tmpData, strstr(buf, "t=") + 2, 5); //获取温度数据
                     float tempC = strtof(tmpData, NULL);  // 格式化处理
                     printf("Device: %s  - ", dev);        // 分离温度数据
                     printf("Temp: %.3f C  ", tempC / 1000); //转换为摄氏度
                     printf("%.3f F\n\n", (tempC / 1000) * 9 / 5 + 32); // 转换为华氏度
              }
              close(fd); // 关闭设备
       } 
       return 0; 
}
