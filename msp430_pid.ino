#define G_LED P1_0


//m1: pwm: p2_4, dir: p2_1
//m2: pwm: p2_6, dir: p2_7
#define M_PWM P2_6
#define M_DIR P2_7

//m1: a: p1_3, b: p1_4
//m2: a: p2_2, b: o2_3
#define QUAD_A P2_2
#define QUAD_B P2_3

#define GRAPH_MILLIS_PERIOD 10
#define CTRL_MICROS_PERIOD 10

volatile int32_t kp_numer = 3;
volatile int32_t kp_denom = 128; 

volatile int32_t ki_numer = 0;
volatile int32_t ki_denom = 10000000; 

volatile uint8_t m1_state_number = 0;
volatile int32_t steps = 0;
int32_t last_error = 0;

volatile int32_t target = 5000;

int32_t int_error = 0;

bool done = false;

unsigned long graph_last_millis = 0;
unsigned long ctrl_last_micros = 0;


// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(9600);
  pinMode(G_LED, OUTPUT);
  digitalWrite(G_LED, HIGH);
  
  pinMode(M_PWM, OUTPUT);     
  pinMode(M_DIR, OUTPUT);
  pinMode(QUAD_A, INPUT);
  pinMode(QUAD_B, INPUT);

  //pinMode(SW, INPUT_PULLUP);

  analogWrite(M_PWM, 0);
  digitalWrite(M_DIR, LOW);

  uint8_t A = digitalRead(QUAD_A);
  uint8_t B = digitalRead(QUAD_B);

  if (A == HIGH) {
    m1_state_number |= 0x2;
  }
  if (B == HIGH) {
    m1_state_number |= 0x1;
  }
  
  attachInterrupt(QUAD_A, isr_quad_a, CHANGE);
  attachInterrupt(QUAD_B, isr_quad_b, CHANGE);

}

// the loop routine runs over and over again forever:
void loop() {
  

  if (!done) {

    if (micros() - ctrl_last_micros > CTRL_MICROS_PERIOD) {

      ctrl_last_micros = micros();
      
      int32_t error;
      noInterrupts();
      error = target - steps;
      interrupts();

      if (abs(last_error - error) < 10 && error < 100) {
        done= true;
      }
      last_error = error;
  
      int_error += error;
  
      int32_t output = kp_numer * error / kp_denom;
  
      digitalWrite(M_DIR, output < 0 ? LOW : HIGH);
  
      analogWrite(M_PWM, constrain(abs(output), 50, 255));

    }
  }
  else { 
    analogWrite(M_PWM, 0);
    digitalWrite(M_DIR, LOW);
    digitalWrite(G_LED, LOW);
  }
  
  if (millis() - graph_last_millis > GRAPH_MILLIS_PERIOD) {
    Serial.println(steps);
    graph_last_millis = millis();
  }
  
}

void isr_quad_a() {
  steps += (0 - (m1_state_number & 1)) | 1;
  m1_state_number = m1_state_number ^ 0x1;
}

void isr_quad_b() {
  steps -= (0 - (m1_state_number & 1)) | 1;
  m1_state_number = m1_state_number ^ 0x3;
}
