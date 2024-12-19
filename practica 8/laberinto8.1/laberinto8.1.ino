
#include <Arduino.h>
#include <Servo.h>

/**
 * Laberinto8.1 - Grupo L2-A5
 *
 * Alvaro Garcia Fernandez - UO289295
 * Daniel Andina Pailos - UO287791
 * Enol Rodriguez Hevia - UO287935
 */

#define SERVO_LEFT 9
#define SERVO_RIGHT 8

#define SENSOR_LEFT_1 2
#define SENSOR_LEFT_2 10

#define SENSOR_RIGHT_1 3
#define SENSOR_RIGHT_2 11

#define FORWARD_LEFT 150
#define FORWARD_RIGHT 30

#define BACKWARD_LEFT 30
#define BACKWARD_RIGHT 150

#define STOP 90

Servo servoRight;
Servo servoLeft;

void turnRightOnIntersection();
void turnLeftOnIntersection();
void turnRight();
void turnLeft();
void turnBack();
void goForward();
void stop();

void setup() {
  pinMode(SENSOR_LEFT_1, INPUT);
  pinMode(SENSOR_LEFT_2, INPUT);

  pinMode(SENSOR_RIGHT_1, INPUT);
  pinMode(SENSOR_RIGHT_2, INPUT);

  servoRight.attach(SERVO_RIGHT);
  servoLeft.attach(SERVO_LEFT);

  delay(1000);
}

void loop() {
  int right_in = digitalRead(SENSOR_RIGHT_1);
  int left_in = digitalRead(SENSOR_LEFT_1);
  int right_out = digitalRead(SENSOR_RIGHT_2);
  int left_out = digitalRead(SENSOR_LEFT_2);

  if (right_in == HIGH && left_in == HIGH && right_out == HIGH &&
      left_out == HIGH) {
    delay(270);
    left_in = digitalRead(SENSOR_LEFT_1);
    left_out = digitalRead(SENSOR_LEFT_2);
    right_in = digitalRead(SENSOR_RIGHT_1);
    right_out = digitalRead(SENSOR_RIGHT_2);

    if (left_out != HIGH || right_out != HIGH)
      turnRightOnIntersection();
    else
      stop();
  }
  if (right_in == HIGH && left_in == HIGH && right_out == HIGH)
    turnRightOnIntersection();
  else if (right_in == HIGH && left_in == HIGH)
    goForward();
  else if (right_in == HIGH && left_in == LOW)
    turnLeft();
  else if (left_in == HIGH && right_in == LOW)
    turnRight();
  else
    turnBack();
}

void turnRight() {
  servoLeft.write(BACKWARD_LEFT);
  servoRight.write(FORWARD_RIGHT);
}

void turnLeft() {
  servoLeft.write(FORWARD_LEFT);
  servoRight.write(BACKWARD_RIGHT);
}

void turnBack() {
  servoRight.write(FORWARD_RIGHT);
  servoLeft.write(BACKWARD_LEFT);
}

void turnRightOnIntersection() {
  delay(200);
  servoRight.write(BACKWARD_RIGHT);
  servoLeft.write(FORWARD_LEFT);
  delay(500);
  while (digitalRead(SENSOR_LEFT_1) == LOW ||
         digitalRead(SENSOR_RIGHT_1) == LOW) {
  }
}

void goForward() {
  servoRight.write(FORWARD_RIGHT);
  servoLeft.write(FORWARD_LEFT);
}

void stop() {
  servoRight.write(STOP);
  servoLeft.write(STOP);
  exit(0);
}