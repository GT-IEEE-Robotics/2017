# -*- coding: utf-8 -*-
"""
Hey guys this is a quick script that parses the measured data,
puts it into 2d array and then stores it in a dictionary called device_data.
To see the keys of the device data print device_data.keys()
A 2d array should be returned where each row is a measurement of several 
periods over time

Try to see if you can classify the data based on these measurements, have fun!

Afterwards the first entry is plotted for visualization
Let me know if you have any questions on the slack
"""
import os
import numpy as np
import matplotlib.pyplot as plt

dir_name = 'samples' #samples folder
files = os.listdir(dir_name)

## parse raw text output into datastructure
device_data = {}
for dev in files:
    f = file(dir_name + '\\' + dev)
    device_name = f.readline().rstrip()
    data = []
    row = []
    for line in f:
        if line == '\n':
            continue
        elif '* START SAMPLES' in line:
            row = []
        elif '* STOP SAMPLES' in line:
            if (len(data)==0):
                data = row[:]
            else:
                data = np.vstack([data, row])
        elif line.rstrip().lstrip("-+").isdigit():
                row.append(int(line))
    device_data[device_name] = data


## plot first entry of each device type
i = 0
for key in device_data.keys():    
    print key
    cur_dev = device_data[key][0]
    print cur_dev
    #integrate each device
    avg = np.mean(cur_dev)    
    mx = np.max(cur_dev)
    plt.subplot(len(device_data), 1, i+1)
    plt.plot(device_data[key][0])
    plt.gca().set_ylim([-1050,1050])
    plt.title(key+' avg: '+str(avg) + ' max: '+str(mx), fontsize = 16)
    i += 1    
plt.subplots_adjust(hspace=.5)
plt.show()

## put some logic to determine which device is which (choose a tolerance range too)

