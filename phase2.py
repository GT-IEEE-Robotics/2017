import Adafruit_BBIO.PWM as PWM
import Adafruit_BBIO.ADC as ADC
import time

ADC.setup()

ADC1 = "P9_40"  # RIGHT
ADC2 = "P9_39"  # OFF AXIS
ADC3 = "P9_38"  # LEFT
FILTER_THRESHOLD = 0.003
FILTER_TIMES_HIGH = 4
FILTER_TIMES_LOW = 8
RECALIBRATE_THRESHOLD = 25
COUNTS_PER = 20

# f = open("halleffect_dump12", "w")

servo_pin = "P8_13"

arr = [15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1]

def hallEffectReading(baseline):
    # return ((ADC.read(ADC2) + ADC.read(ADC3)) / 2) - ADC.read(ADC2)
    tmp1 = ADC.read(ADC2)
    tmp2 = ADC.read(ADC3)
    return ((tmp1 if (tmp1 > tmp2) else tmp2) - ADC.read(ADC1)) - baseline

def calibrateHallEffect():
    print("Calibrating hall effect sensor from")
    i = baseline = 0
    while (i < 100):
            baseline += hallEffectReading(0)
            i+=1
    return baseline / 100

hallEffectBaseline = calibrateHallEffect()

# f.write(str(hallEffectBaseline) + "\n")

lowCounter = 0
highCounter = 0
recalibrateCounter = 0
HIT = 0
currentState = 0
PWM.start(servo_pin, 2.0, 40.0)


while (1):
    i = 0
    hallEffectCurrent = 0

    # get the current value which is an average of 20 counts
    while (i < COUNTS_PER):
        hallEffectCurrent += hallEffectReading(hallEffectBaseline)
        i+=1
    hallEffectCurrent /= COUNTS_PER

    if (currentState == 0): # if we are in a low state
        if (hallEffectCurrent > FILTER_THRESHOLD):  # if the reading is > threshold
            highCounter += 1
        else:
            lowCounter += 1
        if (hallEffectCurrent < -FILTER_THRESHOLD):
            recalibrateCounter += 1
            print(recalibrateCounter)
            if (recalibrateCounter > RECALIBRATE_THRESHOLD):
                hallEffectBaseline = calibrateHallEffect()
                recalibrateCounter = 0
        elif (recalibrateCounter > 0):
            recalibrateCounter -= 2
        if (highCounter > FILTER_TIMES_HIGH):
            HIT = 1
            print("HIT")
            highCounter = 0
            currentState = 1
            for duty in range (2,7):
                PWM.set_duty_cycle(servo_pin, duty)
                time.sleep(0.05)
            #                            print (duty)
            for duty in range (6, 1, -1):
                PWM.set_duty_cycle(servo_pin, duty)
                time.sleep(0.05)
            #                            print(duty)

    else:
        HIT = 0
        if (hallEffectCurrent < FILTER_THRESHOLD):
            lowCounter += 1
        else:
            lowCounter = 0
        if (lowCounter > FILTER_TIMES_LOW):
            currentState = 0
            print("LOW")

    # f.write(str(hallEffectCurrent) + "," + str(HIT) + "\n")

