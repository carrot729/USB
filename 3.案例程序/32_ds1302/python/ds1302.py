import time
import RPi.GPIO
from datetime import datetime

# DS1302类
class makerobo_DS1302:

    ds_CLK_PERIOD = 0.00001

    ds_DOW = [ "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" ]


    def __init__(self, scl=23, rst=25, io=24):
        self.scl = scl
        self.rst = rst
        self.io = io
        # 关闭GPIO警告。.
        RPi.GPIO.setwarnings(False)
        # 配置树莓派GPIO接口.
        RPi.GPIO.setmode(RPi.GPIO.BCM)
        # 初始化ds1302模块.
        self.init_ds1302()
        # 确保write protect已关闭.
        self.write_byte(int("10001110", 2))
        self.write_byte(int("00000000", 2))
        # 确保涓流充电模式被关闭。.
        self.write_byte(int("10010000", 2))
        self.write_byte(int("00000000", 2))
        # 关闭DS1302模块.
        self.end_ds1302()
        self.datetime = {}

    def CloseGPIO(self):
        '''
        释放资源.
        '''
        RPi.GPIO.cleanup() # 释放资源


    def init_ds1302(self):
        '''
        启动ds1302 工作.
        '''
        RPi.GPIO.setup(self.scl, RPi.GPIO.OUT, initial=0)
        RPi.GPIO.setup(self.rst, RPi.GPIO.OUT, initial=0)
        RPi.GPIO.setup(self.io, RPi.GPIO.OUT, initial=0)
        RPi.GPIO.output(self.scl, 0)
        RPi.GPIO.output(self.io, 0)
        time.sleep(self.ds_CLK_PERIOD)
        RPi.GPIO.output(self.rst, 1)


    def end_ds1302(self):
        '''
        ds1302 结束工作.
        '''
        RPi.GPIO.setup(self.scl, RPi.GPIO.OUT, initial=0)
        RPi.GPIO.setup(self.rst, RPi.GPIO.OUT, initial=0)
        RPi.GPIO.setup(self.io, RPi.GPIO.OUT, initial=0)
        RPi.GPIO.output(self.scl, 0)
        RPi.GPIO.output(self.io, 0)
        time.sleep(self.ds_CLK_PERIOD)
        RPi.GPIO.output(self.rst, 0)


    def write_byte(self, Byte):
        '''
        将一个字节的数据写入DS1302 RTC。
        '''
        for Count in range(8):
            time.sleep(self.ds_CLK_PERIOD)
            RPi.GPIO.output(self.scl, 0)

            Bit = Byte % 2
            Byte = int(Byte / 2)
            time.sleep(self.ds_CLK_PERIOD)
            RPi.GPIO.output(self.io, Bit)

            time.sleep(self.ds_CLK_PERIOD)
            RPi.GPIO.output(self.scl, 1)
        
    def read_byte(self):
        '''
        将一个字节的数据读入DS1302 RTC。
        '''
        RPi.GPIO.setup(self.io, RPi.GPIO.IN, pull_up_down=RPi.GPIO.PUD_DOWN)

        Byte = 0
        for Count in range(8):
            time.sleep(self.ds_CLK_PERIOD)
            RPi.GPIO.output(self.scl, 1)

            time.sleep(self.ds_CLK_PERIOD)
            RPi.GPIO.output(self.scl, 0)
            
            time.sleep(self.ds_CLK_PERIOD)
            Bit = RPi.GPIO.input(self.io)
            Byte |= ((2 ** Count) * Bit)

        return Byte


    def write_ram(self, Data):
        '''
        向RTC RAM写入信息.
        '''
        # 初始化ds1302 模块
        self.init_ds1302()
        # 写入地址信息.
        self.write_byte(int("11111110", 2))
        # 写入数据信息.
        for Count in range(len(Data)):
            self.write_byte(ord(Data[Count:Count + 1]))
        for Count in range(31 - len(Data)):
            self.write_byte(ord(" "))
        # 结束DS1302.
        self.end_ds1302()

    def read_ram(self):
        '''
        从RTC RAM中读取信息.
        '''
        # 启动ds1302模块.
        self.init_ds1302()
        # 写入地址信息.
        self.write_byte(int("11111111", 2))
        # 写入数据信息.
        Data = ""
        for Count in range(31):
            Byte = self.read_byte()
            Data += chr(Byte)
        # 结束ds1302通讯.
        self.end_ds1302()
        return Data


    def set_datetime(self, dt):
        '''
        写日期和时间给RTC.
        '''
        if not self.check_sanity():
            return False
        # 初始化ds1302模块.
        self.init_ds1302()
        # 写入地址信息.
        self.write_byte(int("10111110", 2))
        # 写入时钟秒数据.
        self.write_byte((dt.second % 10) | int(dt.second / 10) * 16)
        # 写入时间分数据.
        self.write_byte((dt.minute % 10) | int(dt.minute / 10) * 16)
        # 写入时间时数据.
        self.write_byte((dt.hour % 10) | int(dt.hour / 10) * 16)
        # 写入时间日数据.
        self.write_byte((dt.day % 10) | int(dt.day / 10) * 16)
        # 写入时间月信息.
        self.write_byte((dt.month % 10) | int(dt.month / 10) * 16)
        # 写入星期数据信息.
        self.write_byte((dt.isoweekday() % 10) | int(dt.isoweekday() / 10) * 16)
        # 写入时间年信息.
        self.write_byte((dt.year % 100 % 10) | int(dt.year % 100 / 10) * 16)
        # 确保write protect已关闭.
        self.write_byte(int("00000000", 2))
        # 确保涓流充电模式被关闭.
        self.write_byte(int("00000000", 2))
        # 结束DS1302工作.
        self.end_ds1302()


    def get_datetime(self):
        '''
        从RTC中读取日期和时间.
        '''
        # 启动DS1302的沟通.
        self.init_ds1302()
        # 写入地址信息.
        self.write_byte(int("10111111", 2))
        # 读取时间数据.
        Data = ""

        Byte = self.read_byte()  
        second = (Byte % 16) + int(Byte / 16) * 10   
        Byte = self.read_byte()
        minute = (Byte % 16) + int(Byte / 16) * 10
        Byte = self.read_byte()
        hour = (Byte % 16) + int(Byte / 16) * 10
        Byte = self.read_byte()
        day = (Byte % 16) + int(Byte / 16) * 10
        Byte = self.read_byte()
        month = (Byte % 16) + int(Byte / 16) * 10
        Byte = self.read_byte()
        day_of_week = ((Byte % 16) + int(Byte / 16) * 10) - 1
        Byte = self.read_byte()
        year = (Byte % 16) + int(Byte / 16) * 10 + 2000

        # 结束ds1302通讯.
        self.end_ds1302()
        return datetime(year, month, day, hour, minute, second)

    def check_sanity(self):
        "makerob check sanity of a clock. returns True if clock is sane and False otherwise"
        dt = self.get_datetime()
        if dt.year == 2000 or dt.month == 0 or dt.day == 0:
            return False
        if dt.second == 80:
            return False
        return True

def format_time(dt):
    if dt is None:
        return ""
    fmt = "%m/%d/%Y %H:%M"
    return dt.strftime(fmt)
    
def parse_time(s):
    fmt = "%m/%d/%Y %H:%M"
    return datetime.strptime(s, fmt)

