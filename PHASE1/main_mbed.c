#include "mbed.h"
#include "uLCD_4DGL.h"

#define PHASE_DELAY 1

void printTheResult(int thepad, char thechar);

Serial msp430(p9, p10); // tx, rx to arduino, only RX matters right now so Arduino TX -> MBED RX (10)
//Serial pc(USBTX, USBRX); // tx, rx from PC host
DigitalOut myled(LED1);
uLCD_4DGL uLCD(p28, p27, p29);

char outputs[5] = {'6', '6', '6', '6', '6'};
int continueLoop = 1;

int main() {
    
    // init
//    pc.baud(115200);
    msp430.baud(9600);
    uLCD.baudrate(BAUD_115200);
    wait(PHASE_DELAY);
    
    // start the code
    while (continueLoop) {
        uLCD.cls();
        msp430.putc('b');   // tell the msp430 to begin
        int i = 0;
        char buf[25];
        while (i < 25) {
            if(msp430.readable()) {
    //            pc.putc(msp430.getc());
                buf[i] = msp430.getc();
                i++;
            }
        }
        for (i = 0; i < 5; i++) {
            if (outputs[i] == '6')
                outputs[i] = buf[(i*5)+2];
            printTheResult(i, outputs[i]);
        }
        continueLoop = 0;
        for (i = 0; i < 5; i++) {
            if (outputs[i] == '6')
                continueLoop = 1;
        }
        wait(1);
    }
}

void printTheResult(int thepad, char thechar) {
    if (thechar == '0')
        uLCD.printf("Pad %d: %s\n", thepad, "wire");
    else if (thechar == '1')
        uLCD.printf("Pad %d: %s\n", thepad, "resistor");
    else if (thechar == '2')
        uLCD.printf("Pad %d: %s\n", thepad, "capacitor");
    else if (thechar == '3')
        uLCD.printf("Pad %d: %s\n", thepad, "inductor");
    else if (thechar == '4')
        uLCD.printf("Pad %d: %s\n", thepad, "diode");
    else if (thechar == '5')
        uLCD.printf("Pad %d: %s\n", thepad, "diode");
    else if (thechar == '6')
        uLCD.printf("Pad %d: %s\n", thepad, "open");
}
