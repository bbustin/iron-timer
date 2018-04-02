// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>

#define PIN_RESET_BTN      2
#define PIN_RELAY          A0
//#define PIN_OFF_BTN        3

#define PIN_ANODE_TENS     12
#define PIN_ANODE_ONES     13

// common standard is alphabetic starting at top and going clockwise
#define PIN_A               5
#define PIN_B               6
#define PIN_C               10
#define PIN_D               8
#define PIN_E               7
#define PIN_F               4
#define PIN_G               9
#define PIN_DP              11

// the number of pins we are using as the upper bound for loops
//// this might help from trying to read or write past an array's size
#define NUM_DIGIT_PINS  8

// what each number in the countdown represents in MS
#define PERIOD_MS       60000

// max time units to reset to
#define MAX_TIME        20

// For common anode two digit seven segment display
// number pins LOW means on and HIGH means off

// array with all the pins in the same order as specified above
const int pinNumbers[NUM_DIGIT_PINS] = {
  PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G, PIN_DP
};

// define how to create digits 0 through 9
//// 1 means on and 0 means off (1 will pull pin LOW and 0 HIGH since it is a common anode display)
const int numberDefinitions[][NUM_DIGIT_PINS] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}, // 9
};

const int digitOffDefinition[NUM_DIGIT_PINS] = {0,0,0,0,0,0,0,0};

unsigned long lastTimeNumberChanged; //we will use this to calculate when a minute has elapsed and then decrease the counter

volatile int timeLeft = 0;
volatile bool tensOn = false;
volatile bool tensDecimalPoint = false; // the heartbeat will be alternating decimal points, false here means the ones decimal point will be illuminated

void setup() {
  cli();             // disable global interrupts
  TCCR1A = 0;        // set entire TCCR1A register to 0
  TCCR1B = 0;
  
  // enable Timer1 overflow interrupt:
  TIMSK1 = (1 << TOIE1);
  // Set CS10 bit so timer runs at clock speed:
  TCCR1B |= (1 << CS10);
  // enable global interrupts:
  sei();
  
  Serial.begin(115200);
  // set digit pins to output and set to off (HIGH) by default

  for (int i=0; i < NUM_DIGIT_PINS; i++) {
    int digitPin = pinNumbers[i];
    pinMode(digitPin, OUTPUT);
    digitalWrite(digitPin, HIGH);
  }

  pinMode(PIN_ANODE_TENS, OUTPUT);
  pinMode(PIN_ANODE_ONES, OUTPUT);

  pinMode(PIN_RESET_BTN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_RESET_BTN), reset_time, LOW);

  pinMode(PIN_RELAY, OUTPUT);
  
//  pinMode(PIN_OFF_BTN, INPUT);
}

ISR(TIMER1_OVF_vect) {
  display_number(timeLeft);
}

void reset_time() {
  timeLeft = MAX_TIME;
}

void end_time() {
  timeLeft = 0;
}

void loop() {
  tensDecimalPoint = !tensDecimalPoint;

//  if (digitalRead(PIN_OFF_BTN) == 1) {
//    timeLeft = 0;
//  }
  
  if (timeLeft > 0) {
    digitalWrite(PIN_RELAY, HIGH);
    run_timer();
  } else {
    digitalWrite(PIN_RELAY, LOW);
  }
  
  delay(100);
}

void run_timer() {
  unsigned long now = millis();
  if (now - lastTimeNumberChanged >= PERIOD_MS || now - lastTimeNumberChanged < 0) {
    lastTimeNumberChanged = now;
    timeLeft = timeLeft - 1;
  }
}

void display_number(int number) {
  tensOn = !tensOn;

  if (tensOn) {
    digitalWrite(PIN_ANODE_ONES, LOW);
    
    if (number > 9) {
      int tens = number / 10;
      set_digit_pins(numberDefinitions[tens], tensDecimalPoint);
    } else {
      set_digit_pins(digitOffDefinition, tensDecimalPoint);
    }

    digitalWrite(PIN_ANODE_TENS, HIGH);
  } else {
    digitalWrite(PIN_ANODE_TENS, LOW);

    if (number > 0) {
      int ones = number % 10;
      set_digit_pins(numberDefinitions[ones], !tensDecimalPoint);
    } else {
      set_digit_pins(digitOffDefinition, !tensDecimalPoint);
    }
    
    digitalWrite(PIN_ANODE_ONES, HIGH);
  }
}

void set_digit_pins(int data[NUM_DIGIT_PINS], bool decimalPointOn) {
  for (int i=0; i < NUM_DIGIT_PINS; i++) {
    int digitPin = pinNumbers[i];
    int value = data[i];

    if (decimalPointOn && i == NUM_DIGIT_PINS - 1) {
      value = 1;
    }

    if (value == 1) {
      digitalWrite(digitPin, LOW);
    } else {
      digitalWrite(digitPin, HIGH); 
    }
  }
}

void display_debug() {
  digitalWrite(PIN_ANODE_TENS, HIGH);
  digitalWrite(PIN_ANODE_ONES, HIGH);
  
  Serial.println("PIN_A");
  digitalWrite(PIN_A, LOW);
  delay(2500);
  digitalWrite(PIN_A, HIGH);

  Serial.println("PIN_B");
  digitalWrite(PIN_B, LOW);
  delay(2500);
  digitalWrite(PIN_B, HIGH);

  Serial.println("PIN_C");
  digitalWrite(PIN_C, LOW);
  delay(2500);
  digitalWrite(PIN_C, HIGH);

  Serial.println("PIN_D");
  digitalWrite(PIN_D, LOW);
  delay(2500);
  digitalWrite(PIN_D, HIGH);

  Serial.println("PIN_E");
  digitalWrite(PIN_E, LOW);
  delay(2500);
  digitalWrite(PIN_E, HIGH);

  Serial.println("PIN_F");
  digitalWrite(PIN_F, LOW);
  delay(2500);
  digitalWrite(PIN_F, HIGH);

  Serial.println("PIN_G");
  digitalWrite(PIN_G, LOW);
  delay(2500);
  digitalWrite(PIN_G, HIGH);

  Serial.println("PIN_DP");
  digitalWrite(PIN_DP, LOW);
  delay(2500);
  digitalWrite(PIN_DP, HIGH);
}
