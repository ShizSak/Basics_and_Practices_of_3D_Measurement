#!/usr/bin/env python
# -*- coding: utf-8 -*-
import pigpio
import VL53L0X
import socket
import time

class M5Servo180:
    def __init__(self, pin=None):
        self.range_of_motion = 180
        self.min_pulse_width = 500
        self.max_pulse_width = 2500
        self.pig = pigpio.pi()
        self.pin = pin

    def move(self, theta):
        spw = (theta/self.range_of_motion) * (self.max_pulse_width-self.min_pulse_width) + self.min_pulse_width
        self.pig.set_servo_pulsewidth(self.pin, spw)

    def stop(self):
        self.pig.set_servo_pulsewidth(self.pin, 0)

addr = ("127.0.0.1", 9999)
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setblocking(False)

tof = VL53L0X.VL53L0X(i2c_bus=1,i2c_address=0x29)
tof.open()
tof.start_ranging(VL53L0X.Vl53l0xAccuracyMode.HIGH_SPEED)

servo = M5Servo180(18)
servo.move(0.0)
time.sleep(1.0)

cnt = 0
a = 0.0
try:
    while True:
        distance = tof.get_distance()
        data = "%d, %d" % (a, distance)
        print(data)
        s.sendto(data.encode(), addr)
        cnt = cnt + 1
        a = (float(cnt) * 2.0) % 360
        a = 360 - a if a >= 180 else a
        servo.move(a)
        time.sleep(0.020)

except KeyboardInterrupt:
    tof.stop_ranging()
    tof.close()
    servo.stop()

s.close()
