#include <ESP32Servo.h>

#define RXD2 16
#define TXD2 17
#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO1_PIN 13
#define SERVO2_PIN 12
#define DISTANCE_THRESHOLD_CM 10.0 

Servo servoFlap;
Servo servoRelease;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  servoFlap.attach(SERVO1_PIN);
  servoRelease.attach(SERVO2_PIN);
  
  servoFlap.write(90);
  servoRelease.write(0);
}

float getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999.0;
  return (duration * 0.0343) / 2.0;
}

void processSegregation(String category) {
  if (category == "ORGANIC") {
    servoFlap.write(45);
  } else if (category == "RECYCLABLE") {
    servoFlap.write(90);
  } else if (category == "HAZARDOUS") {
    servoFlap.write(135);
  } else {
    return;
  }
  
  delay(500);
  servoRelease.write(90);
  delay(1000);
  servoRelease.write(0);
  delay(500);
  servoFlap.write(90);
}

void loop() {
  float distance = getDistanceCM();
  
  if (distance > 0 && distance <= DISTANCE_THRESHOLD_CM) {
    delay(500);
    Serial2.println("CAPTURE");
    
    unsigned long timeout = millis();
    while (!Serial2.available() && millis() - timeout < 5000) {
      delay(10);
    }
    
    if (Serial2.available()) {
      String category = Serial2.readStringUntil('\n');
      category.trim();
      processSegregation(category);
    }
    delay(2000);
  }
  delay(60);
}
