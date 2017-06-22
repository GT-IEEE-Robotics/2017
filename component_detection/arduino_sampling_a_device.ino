// 10 microseconds is 16 clock cycles at 16 MHz on ATmega328 on the Arduino Uno
#define SAMPLING_PERIOD_US 10 
#define PWM_NUM_SAMPLES 10
#define BUF_LEN 500

#define PWM_PIN1 10
#define PWM_PIN2 11

unsigned long t;
unsigned long num_samples;
int index = 0;
int samples[BUF_LEN] = {0};

bool phase_status = false;

void setup() {
 pinMode(PWM_PIN1, OUTPUT);
 pinMode(PWM_PIN2, OUTPUT);
 pinMode(13, OUTPUT);
 digitalWrite(13, LOW);
 digitalWrite(PWM_PIN1, HIGH);
 digitalWrite(PWM_PIN2, LOW);
 Serial.begin(115200);
 t = micros();
}

void loop() {

// count time until 10 us since last time measured. then sampling period is over
 if (micros() - t > SAMPLING_PERIOD_US) {
   // sample with adc. (should be taking around 16 sample in a sampling period)
   // since reference flips back and forth,
   // coordinate by flipping measured voltage value
   samples[index] = phase_status ? analogRead(A5) : -analogRead(A5);
   index++;
   t = micros();

    // after 10 samples clock the reference and component signal
   if ((index % PWM_NUM_SAMPLES) == 0) {
     if (phase_status) {
       digitalWrite(PWM_PIN1, HIGH);
       digitalWrite(PWM_PIN2, LOW);
     }
     else {
       digitalWrite(PWM_PIN1, LOW);
       digitalWrite(PWM_PIN2, HIGH);
     }
     // flip which direction is positive
     phase_status = !phase_status;
     digitalWrite(13, phase_status ? HIGH : LOW);
   }

   // after we've filled the buffer spit it out, wait 1 sec, and reset time counter
   if (index >= BUF_LEN) {
     index = 0;
     Serial.println("* START SAMPLES");
     for (int j = 0; j < BUF_LEN; j++) {
       Serial.println(samples[j]);
     }
     Serial.println("* STOP SAMPLES");
     delay(1000);
     t = micros();

   }
   
 }
}