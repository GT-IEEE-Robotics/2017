#!/usr/bin/python
import Adafruit_BBIO.GPIO as gpio
import Adafruit_BBIO.PWM as pwm

def motor_control(d, p): 
        pwm_init() # initialize devices 

        # task: given "ccw" or "cw" for parameter d
        #       drive the motors at "ccw" and "cw"
        #       assume when A=0, B=1, motor turns ccw.
        # write your code here




def pwm_init():
        gpio.setup("P9_12", gpio.OUT)
        pwm.start("P9_14", 0, 1000)

def motor_test():
        # write high to "B"
        gpio.setup("P9_12", gpio.HIGH)
        # write low to "B"
        gpio.setup("P9_12", gpio.LOW)
        # write 39 percent to "A"
        pwm.set_duty_cycle("P9_14", 39)


        # write 0 percent to "A"
        pwm.set_duty_cycle("P9_14", 0)
