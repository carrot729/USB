/****************************************
# －－－－湖南创乐博智能科技有限公司－－－－
#  文件名：32_rtc_ds1302.c
#  版本：V2.0
#  author: zhulin
#  说明：DS1302 实时时钟模块实验
*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <wiringPi.h>
#include <ds1302.h>

// 寄存器定义

#define	makerobo_RTC_SECS	   0     // 秒   
#define	makerobo_RTC_MINS	   1     // 分
#define	makerobo_RTC_HOURS	 2     // 小时
#define	makerobo_RTC_DATE	   3     // 天
#define	makerobo_RTC_MONTH	 4     // 月
#define	makerobo_RTC_DAY		 5    
#define	makerobo_RTC_YEAR	   6     // 年
#define	makerobo_RTC_WP		   7
#define	makerobo_RTC_TC		   8
#define	makerobo_RTC_BM		  31


static unsigned int makerobo_masks [] = { 0x7F, 0x7F, 0x3F, 0x3F, 0x1F, 0x07, 0xFF } ;


// BCD码转十进制
static int makerobo_bcdToD (unsigned int ds_byte, unsigned int ds_mask)
{
  unsigned int ds_b1, ds_b2 ;
  ds_byte &= ds_mask ;
  ds_b1 = ds_byte & 0x0F ;
  ds_b2 = ((ds_byte >> 4) & 0x0F) * 10 ;
  return ds_b1 + ds_b2 ;
}

// 十进制转BCD码
static unsigned int makerobo_dToBcd (unsigned int ds_byte)
{
  ds_byte = ds_byte % 100;
  return ((ds_byte / 10) << 4) + (ds_byte % 10) ;
}

// 测试数据
static int makerobo_ramTestValues [] =
  { 0x00, 0xFF, 0xAA, 0x55, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0xF0, 0x0F, -1 } ;

static int makerobo_ramTest (void)
{
  int ds_addr ;
  int ds_got ;
  int ds_i = 0 ;
  int ds_errors = 0 ;
  int ds_testVal ;

  printf ("Makerobo DS1302 RAM TEST\n") ;

  ds_testVal = makerobo_ramTestValues [ds_i] ;  // 获取测试数据

  while (ds_testVal != -1)
  {
    for (ds_addr = 0 ; ds_addr < 31 ; ++ds_addr)
      ds1302ramWrite (ds_addr, ds_testVal) ;

    for (ds_addr = 0 ; ds_addr < 31 ; ++ds_addr)
      if ((ds_got = ds1302ramRead (ds_addr)) != ds_testVal)
      {
	printf (" Makerobo DS1302 RAM Failure: Address: %2d, Expected: 0x%02X, Got: 0x%02X\n",
		ds_addr, ds_testVal, ds_got) ;
	++ds_errors ;
      }
    ds_testVal = makerobo_ramTestValues [++ds_i] ;
  }

  for (ds_addr = 0 ; ds_addr < 31 ; ++ds_addr)
    ds1302ramWrite (ds_addr, ds_addr) ;

  for (ds_addr = 0 ; ds_addr < 31 ; ++ds_addr)
    if ((ds_got = ds1302ramRead (ds_addr)) != ds_addr)
    {
      printf ("Makerobo DS1302 RAM Failure: Address: %2d, Expected: 0x%02X, Got: 0x%02X\n",
	      ds_addr, ds_addr, ds_got) ;
      ++ds_errors ;
    }

  if (ds_errors == 0)
    printf ("-- Makerobo DS1302 RAM TEST: OK\n") ;                         //打印出测试信息
  else
    printf ("-- Makerobo DS1302 RAM TEST FAILURE. %d errors.\n", ds_errors) ;// 打印出错误码

  return 0 ;
}

// 设置Linux时钟
static int makerobo_setLinuxClock (void)
{
  char ds_dateTime [20] ;
  char ds_command [64] ;
  int  ds_clock [8] ;


  printf ("Makerobo Setting the Linux Clock from the DS1302... ") ; fflush (stdout) ;

  ds1302clockRead (ds_clock) ;

// [MMDDhhmm[[CC]YY][.ss]]

  sprintf (ds_dateTime, "%02d%02d%02d%02d%02d%02d.%02d",
	makerobo_bcdToD (ds_clock [makerobo_RTC_MONTH], makerobo_masks [makerobo_RTC_MONTH]),
	makerobo_bcdToD (ds_clock [makerobo_RTC_DATE],  makerobo_masks [makerobo_RTC_DATE]),
	makerobo_bcdToD (ds_clock [makerobo_RTC_HOURS], makerobo_masks [makerobo_RTC_HOURS]),
	makerobo_bcdToD (ds_clock [makerobo_RTC_MINS],  makerobo_masks [makerobo_RTC_MINS]),
	20,
	makerobo_bcdToD (ds_clock [makerobo_RTC_YEAR],  makerobo_masks [makerobo_RTC_YEAR]),
	makerobo_bcdToD (ds_clock [makerobo_RTC_SECS],  makerobo_masks [makerobo_RTC_SECS])) ;

  sprintf (ds_command, "/bin/date %s", ds_dateTime) ;
  system (ds_command) ;

  return 0 ;
}



// 设置DS1302时间:
static int makerobo_setDSclock (void)
{
  struct tm t ;
  time_t now ;
  int ds_clock [8] ;
  int ds_time = 0 ;
  int ds_date = 0 ;
  int ds_weekday = 0 ;

  printf ("Setting the clock in the DS1302 from type in... ") ;

  printf ("\n\nEnter Date(YYYYMMDD): ") ;
  scanf ("%d", &ds_date) ;
  printf ("Enter time(HHMMSS, 24-hour clock): ") ;
  scanf ("%d", &ds_time) ;
  printf ("Enter Weekday(0 as sunday): ") ;
  scanf ("%d", &ds_weekday) ;


  ds_clock [ 0] = makerobo_dToBcd (ds_time % 100) ;	// 秒
  ds_clock [ 1] = makerobo_dToBcd (ds_time / 100 % 100) ;	// 分
  ds_clock [ 2] = makerobo_dToBcd (ds_time / 100 / 100) ;	// 小时
  ds_clock [ 3] = makerobo_dToBcd (ds_date % 100) ;	// 天
  ds_clock [ 4] = makerobo_dToBcd (ds_date / 100 % 100) ;	// 月 0-11 --> 1-12
  ds_clock [ 5] = makerobo_dToBcd (ds_weekday) ;	// 星期 (sun 0)
  ds_clock [ 6] = makerobo_dToBcd (ds_date / 100 / 100) ;       // 年
  ds_clock [ 7] = 0 ;			// 写保护关闭

  ds1302clockWrite (ds_clock) ;

  printf ("OK\n") ;

  return 0 ;
}
// 主函数
int main (int argc, char *argv [])
{
  int i ;
  int ds_clock [8] ;
  int ds_year ;
  int ds_month ;
  int ds_date ;
  int ds_hour ;
  int ds_minute ;
  int ds_second ;
  int ds_weekday ;

  wiringPiSetup () ;        // wiringPi 初始化 
  ds1302setup   (0, 1, 2) ; // ds1302初始化

  // 命令参数解析
  if (argc == 2)
  {
    if (strcmp (argv [1], "-slc") == 0)
      return makerobo_setLinuxClock () ;      // 设置系统时间
    else if (strcmp (argv [1], "-sdsc") == 0)
      return makerobo_setDSclock () ;        // 设置DS1302时间
    else if (strcmp (argv [1], "-rtest") == 0)
      return makerobo_ramTest () ;          // 获取测试时间
    else
    {
      printf ("Usage: ds1302 [-slc | -sdsc | -rtest]\n") ;  // 如果输入错误，弹出提示信息
      return EXIT_FAILURE ;
    }
  }
// 读取具体的时间
  for (i = 0 ;; ++i)
  {
    printf ("%5d:  ", i) ;

    ds1302clockRead (ds_clock) ; // 读取时间
    
	ds_hour   =  makerobo_bcdToD (ds_clock [2], makerobo_masks [2]) ;
	ds_minute =  makerobo_bcdToD (ds_clock [1], makerobo_masks [1]) ;
	ds_second =  makerobo_bcdToD (ds_clock [0], makerobo_masks [0]) ;

  ds_date  =   makerobo_bcdToD (ds_clock [3], makerobo_masks [3]) ;
	ds_month =   makerobo_bcdToD (ds_clock [4], makerobo_masks [4]) ;
	ds_year  =   makerobo_bcdToD (ds_clock [6], makerobo_masks [6]) + 2000 ;
	ds_weekday = makerobo_bcdToD (ds_clock [5], makerobo_masks [5]) ;

	printf ("  %04d-%02d-%02d", ds_year, ds_month, ds_date) ;     // 打印出年/月/日
	printf ("  %02d:%02d:%02d", ds_hour, ds_minute, ds_second) ;  // 打印出时/分/秒
    // 打印出星期信息
    switch (ds_weekday){
      case 0: printf ("  SUN") ; break;
	    case 1: printf ("  MON") ; break;
	    case 2: printf ("  TUE") ; break;
	    case 3: printf ("  WED") ; break;
	    case 4: printf ("  THU") ; break;
	    case 5: printf ("  FRI") ; break;
	    case 6: printf ("  SAT") ; break;
	}
    printf ("\n") ;   // 换行
    delay (200) ;    // 延时200ms
  }
 
  return 0 ;
}
