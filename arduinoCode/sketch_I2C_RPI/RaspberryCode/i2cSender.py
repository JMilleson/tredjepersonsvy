import smbus
import time
bus = smbus.SMBus(0)
address = 0x04

number1 = 111
number2 = -180
float1 = 1.11
float2 = 2.22

while True:
        values = getValues()
        bus.write_i2c_block_data(adress, 0, values)
        print bearing
        print bear255
        time.sleep(1)


def getValues():
        