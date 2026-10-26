#include <Servo.h>

Servo horizontalServo;
Servo verticalServo;

int horizAngle = 90;
int vertAngle = 90;

void setup() {
  horizontalServo.attach(9);
  verticalServo.attach(10);
  horizontalServo.write(horizAngle);
  verticalServo.write(vertAngle);
}

void loop() {
  int tl = analogRead(A0);
  int tr = analogRead(A1);
  int bl = analogRead(A2);
  int br = analogRead(A3);

  int avgTop = (tl + tr) / 2;
  int avgBottom = (bl + br) / 2;
  int avgLeft = (tl + bl) / 2;
  int avgRight = (tr + br) / 2;

  int diffVert = avgTop - avgBottom;
  int diffHoriz = avgLeft - avgRight;

  if (abs(diffVert) > 50) {
    if (avgTop > avgBottom) {
      vertAngle++;
    } else {
      vertAngle--;
    }
    vertAngle = constrain(vertAngle, 0, 180);
    verticalServo.write(vertAngle);
  }

  if (abs(diffHoriz) > 50) {
    if (avgLeft > avgRight) {
      horizAngle++;
    } else {
      horizAngle--;
    }
    horizAngle = constrain(horizAngle, 0, 180);
    horizontalServo.write(horizAngle);
  }

  delay(50);
}
