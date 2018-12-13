#include <Stepper.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(24,26,28,30,32,34);

const int ledPin = 13;      // LED connected to digital pin 13
const int knockSensorS = A0; // the piezo is connected to analog pin 0
const int knockSensorM = A1;
const int knockSensorL = A2;
const int threshold = 100;  // threshold value to decide when the detected sound is a knock or not


// these variables will change:
int sensorReadingS = 0;      // variable to store the value read from the sensor pin
int sensorReadingM = 0;
int sensorReadingL = 0;
int hitS = 0;
int hitM = 0;
int hitL = 0;
int ledState = LOW;         // variable used to store the last LED status, to toggle the light

int in1Pin = 13;
int in2Pin = 12;
int in3Pin = 11;
int in4Pin = 10;
int in5Pin = 9;
int in6Pin = 8;
int in7Pin = 7;
int in8Pin = 6;
int in9Pin = 5;
int in10Pin = 4;
int in11Pin = 3;
int in12Pin = 2;

unsigned long timer = 0;
const int buttonPin = 22;
int lastState = 0;
int buttonState = 0;
int hardMove = 0;
Stepper motorT(512, in1Pin, in2Pin, in3Pin, in4Pin);
Stepper motorL(512, in5Pin, in6Pin, in7Pin, in8Pin);
Stepper motorM(512, in9Pin, in10Pin, in11Pin, in12Pin);

int state = 0;

void setup() {
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(in5Pin, OUTPUT);
  pinMode(in6Pin, OUTPUT);
  pinMode(in7Pin, OUTPUT);
  pinMode(in8Pin, OUTPUT);
  pinMode(in9Pin, OUTPUT);
  pinMode(in10Pin, OUTPUT);
  pinMode(in11Pin, OUTPUT);
  pinMode(in12Pin, OUTPUT);

  pinMode(buttonPin, INPUT);
  // this line is for Leonardo's, it delays the serial interface
  // until the terminal window is opened

  motorM.setSpeed(20);
  motorL.setSpeed(20);
  motorT.setSpeed(20);

  lcd.begin(16, 2);
  lcd.print("SmartDart!");
  lcd.setCursor(0,1);
  lcd.print("Press the button");

  pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
  Serial.begin(9600);       // use the serial port
  
}
void checkReading() {
  // read the sensor and store it in the variable sensorReading:
  sensorReadingS = analogRead(knockSensorS);
  sensorReadingM = analogRead(knockSensorM);
  sensorReadingL = analogRead(knockSensorL);
  // if the sensor reading is greater than the threshold:
  //Serial.println(String(sensorReadingL) + " " + String(sensorReadingS));
  //return;
  if (millis() - timer > 500) {
  if (sensorReadingS >= threshold) {
    // toggle the status of the ledPin:
    timer = millis();
    ledState = !ledState;
    // update the LED pin itself:
    digitalWrite(ledPin, ledState);
    // send the string "Knock!" back to the computer, followed by newline
    hitS++;
    Serial.println("Hit small!");
    if (state == 1){
      lcd.clear();
      lcd.print("Aim for small");
      lcd.setCursor(0,1);
      lcd.print("Score: " + String(hitS));
    }
  }
  if (sensorReadingM >= threshold) {
    // toggle the status of the ledPin:
    timer = millis();
    ledState = !ledState;
    // update the LED pin itself:
    digitalWrite(ledPin, ledState);
    // send the string "Knock!" back to the computer, followed by newline
    hitM++;
    Serial.println("Hit medium!");
    if (state == 2){
      lcd.clear();
      lcd.print("Aim for medium");
      lcd.setCursor(0,1);
      lcd.print("Score: " + String(hitM));
    }
  }
  //Serial.println(sensorReadingL);
  if (sensorReadingL >= threshold) {
    // toggle the status of the ledPin:
    timer = millis();
    ledState = !ledState;
    // update the LED pin itself:
    digitalWrite(ledPin, ledState);
    // send the string "Knock!" back to the computer, followed by newline
    hitL++;
    Serial.println("Hit large!");
    if (state == 3){
      lcd.clear();
      lcd.print("Aim for large");
      lcd.setCursor(0,1);
      lcd.print("Score: " + String(hitL));
    }
  }
}
  // delay to avoid overloading the serial port buffer
}
void moveTargets(int from, int to) {
  if (from == 0) {
    if (to == 1) { 
      lcd.clear();
      motorL.step(-600);
      motorM.step(-700);
      lcd.print("Aim for small");
    }
    if (to == 2) { //medium is right
      lcd.clear();
      motorL.step(-600);
      motorM.step(700);
      lcd.print("Aim for medium");
    } //M to right
    if (to == 3) { //large is right
      lcd.clear();
      motorL.step(600);
      motorM.step(-700); //-700
      lcd.print("Aim for large");
    } 
  }
  else if (from == 1) { //small is right
    if (to == 0) { //back to center
      motorL.step(600);
      motorM.step(700);
    }
    if (to == 2) {
      lcd.clear();
      motorM.step(-1400);
      lcd.print("Aim for medium");
    }
    if (to == 3) { //large is right
      lcd.clear();
      motorL.step(1200);
      lcd.print("Aim for large");
    } 
  }
  else if (from == 2) { //medium is right
    if (to == 0) { //back to center
      motorL.step(600);
      motorM.step(-700);
    }
    if (to == 1) {
      lcd.clear();
      motorM.step(-1400);
      lcd.print("Aim for small");
    }
    if (to == 3) {
      lcd.clear();
      motorL.step(1200);
      motorM.step(-1400);
      lcd.print("Aim for large");
    }
  }
  else if (from == 3) { //large is right
    if (to == 0) { //back to center
      motorL.step(-600);
      motorM.step(700);
    }
    if (to == 1) {
      lcd.clear();
      motorL.step(-1200);
      lcd.print("Aim for small");
    }
    if (to == 2) {
      lcd.clear();
      motorL.step(-1200);
      motorM.step(1400);
      lcd.print("Aim for medium");
  }
  }
}

void loop() {
  checkReading();
  buttonState = digitalRead(buttonPin);
  //Serial.println(buttonState);
  if (!lastState && buttonState) {
    Serial.println("button pressed!");
    switch (state) {
      case 0:
        moveTargets(0, 3);
        Serial.println("moving from 0 to 3");
        hitL = 0;
        state = 3;
        delay(5000);
        break;
      case 1: //small is right
        if (hitS < 3) {
          moveTargets(1, 2);
          Serial.println("moving from 1 to 2");
          motorT.step(-hardMove);
          hitS = 0;
          state = 2;
          hardMove = 0;
          delay(5000);
        }
        if (hitS > 4) {
          if (hardMove <= 1000){
              motorT.step(250);
              hardMove+=250;
              hitS = 0;
              delay(5000);
           }
        }
        break;
      case 2: //medium is left
        if (hitM > 7) {
          moveTargets(2, 1);
          Serial.println("moving from 2 to 1");
          hitM = 0;
          state = 1;
          delay(5000);
        }
        else if (hitM < 4) {
          moveTargets(2, 3);
          Serial.println("moving from 2 to 3");
          motorT.step(-800);
          hitM = 0;
          state = 3;
          delay(5000);
        }
        break;
      case 3:
        if (hitL > 7) {
          moveTargets(3, 2);
          Serial.println("moving from 3 to 2");
          motorT.step(1000);
          hitL = 0;
          state = 2;
          delay(5000);
        }
      break;
      }
  }
  lastState = buttonState;

}

