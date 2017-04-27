#include <Servo.h>
#include "LowPower.h"

const int PIN_SERVO = 9;
const int PIN_BUTTON_STD = 2;
const int PIN_BUTTON_EX = 3;
const int PIN_LED_FLUSHING =  13;

const int FLUSH_STANDARD_DELAY = 3000;
const int FLUSH_EXTENDED_DELAY = 6000;
const int POS_CLOSED = 78;
const int POS_OPEN = 50;

Servo servo;
int buttonStateStd = 0;
int buttonStateEx = 0;  
bool isFlushing = false;

void setup() {
  
  Serial.begin(9600);  
  servo.attach(PIN_SERVO);
  pinMode(PIN_LED_FLUSHING, OUTPUT);
  pinMode(PIN_BUTTON_STD, INPUT);
  pinMode(PIN_BUTTON_EX, INPUT);

  //make sure we're in the closed position
  servo.write(POS_CLOSED);
  delay(500); //we sleep immediately out of this. give time for servo to finish moving
}

void wakeUp() {
  Serial.println("Waking up");
}

void sleep() {
    servo.detach();
    Serial.println("powering down");
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_STD), wakeUp, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_EX), wakeUp, CHANGE);
    delay(100);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(digitalPinToInterrupt(PIN_BUTTON_STD));
    detachInterrupt(digitalPinToInterrupt(PIN_BUTTON_EX));
    servo.attach(PIN_SERVO);
    delay(100);  
}
void loop() {
  //Low Power Save Mode --
  sleep();
  
  buttonStateStd = digitalRead(PIN_BUTTON_STD);
  buttonStateEx = digitalRead(PIN_BUTTON_EX);  
  
  if (buttonStateStd == HIGH) {
    Serial.println("Flush Standard");
    flushToilet(FLUSH_STANDARD_DELAY);    
  } else if (buttonStateEx == HIGH) {
    Serial.println("Flush Extended");
    flushToilet(FLUSH_EXTENDED_DELAY);    
  }
}

void flushToilet(int flushDelay) {
  if (isFlushing) return;

  isFlushing = true;  
  openFlapper();
  delay(flushDelay);  
  closeFlapper();
  isFlushing = false;

  //We sleep after this, we need time for the servo to finish moving.
  delay(100);
}

void openFlapper() {
  Serial.println("Opening Flapper");
  
  servo.write(POS_OPEN);
  
  digitalWrite(PIN_LED_FLUSHING, HIGH);
  delay(500);
  digitalWrite(PIN_LED_FLUSHING, LOW);
  
  Serial.println(servo.read());  
}

void closeFlapper() {
  Serial.println("Closing Flapper");

  digitalWrite(PIN_LED_FLUSHING, HIGH);
  delay(100);
  digitalWrite(PIN_LED_FLUSHING, LOW);
  delay(100);
  digitalWrite(PIN_LED_FLUSHING, HIGH);
  delay(100);
  digitalWrite(PIN_LED_FLUSHING, LOW);
  delay(100);
  digitalWrite(PIN_LED_FLUSHING, HIGH);
  delay(100);
  digitalWrite(PIN_LED_FLUSHING, LOW);
  delay(100);
  digitalWrite(PIN_LED_FLUSHING, HIGH);
  
  servo.write(POS_CLOSED);
  Serial.println(servo.read());

  delay(100);
  digitalWrite(PIN_LED_FLUSHING, LOW);  
}

