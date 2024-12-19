
#include <Arduino.h>
#include <Servo.h>

/**
 * Siguelineas7.1 - Grupo L2-A5
 *
 * Alvaro Garcia Fernandez - UO289295
 * Daniel Andina Pailos - UO287791
 * Enol Rodriguez Hevia - UO287935
 */

#define SERVO_LEFT 8
#define SERVO_RIGHT 9

#define SENSOR_LEFT 2
#define SENSOR_RIGHT 3

#define FORWARD_LEFT 180
#define FORWARD_RIGHT 0

#define BACKWARD_LEFT 0
#define BACKWARD_RIGHT 180

#define STOP 90

Servo servoRight;
Servo servoLeft;

void setup() {
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  servoRight.attach(SERVO_RIGHT);
  servoLeft.attach(SERVO_LEFT);

  delay(1000);
}

void loop() {
  int right = digitalRead(SENSOR_RIGHT);
  int left = digitalRead(SENSOR_LEFT);

  if (right == HIGH && left == HIGH) {
    servoRight.write(FORWARD_RIGHT);
    servoLeft.write(FORWARD_LEFT);
  } else if (right == HIGH && left == LOW) {
    servoLeft.write(FORWARD_LEFT);
    servoRight.write(STOP);
  } else if (left == HIGH && right == LOW) {
    servoLeft.write(STOP);
    servoRight.write(FORWARD_RIGHT);
  } else {
    servoRight.write(BACKWARD_RIGHT);
    servoLeft.write(BACKWARD_LEFT);
  }
}