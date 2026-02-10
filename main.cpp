#include <Arduino.h>
#include "RTClib.h"


const int BUTTON_PIN = 8  ; // pin for button

#define IN1 3          // pin for motor driver
#define IN2 4          // pin for motor driver

RTC_DS3231 rtc;

int lastState = HIGH; // the previous state from the input pin
int currentState;    //
bool motorDirection1 = true;
bool motorDirection2 = false;
bool hasrun = false;
const uint8_t INTERVAL_SECONDS = 60;   

void setup_rtc(){
  if (!rtc.begin()) {
    Serial.println("RTC nicht gefunden!");
    while (1);
  }

  // Falls die Uhr nach einem Batteriewechsel stehen geblieben ist:
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void run_motor(){
  //this turns on the moter with the current direction

      digitalWrite(IN1, motorDirection1);
      digitalWrite(IN2, motorDirection2);
}
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setup_rtc();
  Serial.println("System bereit...");
  run_motor();


}



void stop_motor() {
  //this stops the motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}


void change_motor_direction(){
  // this changes the directoin of the motor
  motorDirection1 = !motorDirection1;
  motorDirection2 = !motorDirection2;
}

void advance_clock(){
  // this advances the clock one minute
    run_motor();
  delay(200);
  change_motor_direction();
  run_motor();
  delay(200);
  stop_motor();
}


void check_clock(uint8_t sec){
   // nur einmal pro Sekunde zählen
    if (((sec % INTERVAL_SECONDS) == 0)&& (hasrun == false)) {
      
      Serial.println("run");
      advance_clock();
      hasrun=true;

      Serial.println(sec);
    }
    if (((sec % INTERVAL_SECONDS) != 0)&& (hasrun == true)) {
      hasrun = false;
      }
      
}

void check_botton_state(){
  currentState = digitalRead(BUTTON_PIN);
  if (currentState == LOW){
    advance_clock();
    Serial.println("motor_move");
    delay(50);
  }
}


void loop() {
  // constantly checks if button was pressed or minute passed
  DateTime now = rtc.now();
  uint8_t sec = now.second();
  
  check_clock(sec);
  check_botton_state();



}