#define UINT32_MAX  0xffffffff

#define SAMPLING_PERIOD_US 0
#define DRIVE_FLIP_IND 16
#define BUF_LEN 64

#define LED P2_6
#define CONSTANT_PWM P2_0
#define ADC P1_0

#define LEN_COMP_ARR 5
uint8_t comp_arr[LEN_COMP_ARR] = { P2_1, P2_2, P2_3, P2_4, P2_5 };

#define NUM_STATS 4
#define NUM_TARGETS 7
uint16_t targets[NUM_TARGETS][NUM_STATS] =
  {{1012, 12, 1015, 15}, // wire
   {189, 834, 192, 836}, // res
   {68, 953, 977, 1020}, // cap
   {740, 268, 804, 1023}, // ind
   {1, 186, 5, 200}, // common anode diode
   {839, 1022, 841, 1023}, // common cathode diode
   {0, 998, 4, 1023}}; // open

unsigned long last_t = 0;
int16_t samples[BUF_LEN] = {0};

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.begin(9600);
} 


void loop() {


  if ( Serial.available() ) {
    digitalWrite(LED, LOW);
    char c = Serial.read();
    if (c >= '1' && c <= '5') {
      test_component(c - '1');
      for (int i = 0; i < BUF_LEN; i++) {
        Serial.println(samples[i]);
      }
    }
    if (c == 'a') {
      for (int i = 0; i < LEN_COMP_ARR; i++) {
        test_component(i);
        uint16_t stats[4] = {0};
        postprocess_buffer(stats);
        Serial.print(stats[0]);
        Serial.print(',');
        Serial.print(stats[1]);
        Serial.print(',');
        Serial.print(stats[2]);
        Serial.print(',');
        Serial.println(stats[3]);
      }
    }
    if (c == 'b') {
      for (int i = 0; i < LEN_COMP_ARR; i++) {
        test_component(i);
        uint16_t stats[4] = {0};
        postprocess_buffer(stats);
        Serial.print(i);
        Serial.print(',');
        Serial.println(disambiguate(stats));
      }
    }
  }
  
}

void test_component(uint8_t comp_index) {

  digitalWrite(LED, LOW);


  pinMode(CONSTANT_PWM, OUTPUT);
  for (uint8_t i = 0; i < LEN_COMP_ARR; i++) {
    pinMode(comp_arr[i], INPUT);
  }
  uint8_t curr_pin = comp_arr[comp_index];
  pinMode(curr_pin, OUTPUT);
  
  int buf_ind = 0;
  int flip_ind = 0;
  bool state = 0;
  int16_t curr_sample;

  digitalWrite(curr_pin, state ? HIGH : LOW);
  digitalWrite(CONSTANT_PWM, state ? LOW : HIGH);

  for (;;) {

    if (buf_ind == BUF_LEN) {
      break;
    }

    if (last_t - micros() > SAMPLING_PERIOD_US) {
      
      if ( flip_ind == DRIVE_FLIP_IND ) {
        state = !state;
        flip_ind = 0;
        digitalWrite(curr_pin, state ? HIGH : LOW);
        digitalWrite(CONSTANT_PWM, state ? LOW : HIGH);
      }
      last_t = micros();
      curr_sample = analogRead(A0);
      samples[buf_ind] = state ? curr_sample : -curr_sample;
      buf_ind++;
      flip_ind++;

    }
    
  }

  digitalWrite(LED, HIGH);

  
}

void postprocess_buffer(uint16_t *stats) {

  uint16_t *pos_avg = stats;
  uint16_t *neg_avg = stats + 1;
  uint16_t *pos_max = stats + 2;
  uint16_t *neg_max = stats + 3;

  for (int i = 0; i < BUF_LEN; i++) {
    if (samples[i] > 0) {
      *pos_avg += samples[i];
      if (samples[i] > *pos_max) {
        *pos_max = samples[i];
      }
    }
    else {
      *neg_avg += (uint16_t)(-samples[i]);
      if ((uint16_t)(-samples[i]) > *neg_max) {
        *neg_max = (uint16_t)(-samples[i]);
      }
    }
  }
  *pos_avg /= (BUF_LEN/2);
  *neg_avg /= (BUF_LEN/2);

}

int8_t disambiguate(uint16_t *stats) {
  int8_t candidate = -1;
  uint32_t max_distance = UINT32_MAX;
  uint32_t cur_distance = 0;

  for (int8_t i = 0; i < NUM_TARGETS; i++) {
    cur_distance = 0;
    for (uint8_t j = 0; j < NUM_STATS; j++) {
       if ( stats[j] > targets[i][j] ) {
          cur_distance += (stats[j] - targets[i][j]) * (stats[j] - targets[i][j]);
       }
       else {
          cur_distance += (targets[i][j] - stats[j]) * (targets[i][j] - stats[j]);
       }
    }
    if (cur_distance < max_distance) {
      candidate = i;
      max_distance = cur_distance;
    }
  }
  
  return candidate;
  
}

