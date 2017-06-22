# interrupt.py - Alexander Hiam - 12/2013
# Sets P9.12 as an input with a pull-up resistor and attaches a 
# falling edge interrupt. The 5th time the pin goes low the interrupt
# is detached.
#
# This example is in the public domain

from bbio import *
from PID import PID
import Adafruit_BBIO.PWM as pwm
from eqep import eQEP

#setup
#pin = GPIO1_28
#gpio.setup("P8_10",gpio.OUT)

n_interrupts = 0

def motorReadAndAdjustment():
  # This function will be called every time the pin goes low
  #from PID
  update_value = pid.update(velocity)

  global n_interrupts
  n_interrupts += 1
  print "interrupt # %i" % n_interrupts
  if n_interrupts % 2 == 0:
    gpio.output("P8_10",gpio.LOW)
  #  detachInterrupt(pin)

def setup():
  pinMode(pin, INPUT, PULLUP)
  attachInterrupt(pin, motorReadAndAdjustment, FALLING)
  print "falling edge interrupt attached to P9.12 (GPIO1_28)"
  pin = GPIO1_28
  gpio.setup("P8_10",gpio.OUT)
  set_mode(self,1)
  set_period(self,100000000)
  set_position(self,0)
  pid = PID(0.2, 0.0, 0.0)
  pid.update()

def loop():
  position = pid.get_position()
  period = pid.get_period()

run(setup, loop)

