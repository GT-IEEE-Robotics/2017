# Component Detection Notes

first set up adc and gpio control on the msp430

match the reference signal for 4 devices you don't plan on measuring. invert the reference signal on the device being measured.

adc sampling rate: 16 samples over 10 micro sec
pwm change rate: 10 samples then change


After you measure a few sample and get to data processing:

- throw out about about 10 first samples
- try to get from start to finish of several periods to have more accurate averages


alternative classification method to explore:
try ramp input with DAC and monitor behavior? 