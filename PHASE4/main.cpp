// Use a mosfet to control a powered tape measure

#include "mbed.h"

DigitalOut myled(LED1);
DigitalOut thepin(p10);

int main() {
    while(1) {
        myled = 1;
        thepin = 1;
        wait(3);
        myled = 0;
        thepin = 0;
        wait(3);
    }
}

