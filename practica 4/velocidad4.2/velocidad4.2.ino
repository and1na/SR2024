
#include <Servo.h>

/**
 * Velocidad4.2 - Grupo L2-A5
 *
 * Alvaro Garcia Fernandez - UO289295
 * Daniel Andina Pailos - UO287791
 * Enol Rodriguez Hevia - UO287935
 */

#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define THRESHOLD 50

#define SERVO 6

Servo servo;
const int POTENTIOMETER_BASE_VALUE = 1024/2;

void setup()
{
  Serial.begin(9600);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);

  servo.attach(SERVO);
}

void loop()
{
  const int x = analogRead(JOYSTICK_X);

  if (x >= POTENTIOMETER_BASE_VALUE + THRESHOLD)
      servo.write(map(x, POTENTIOMETER_BASE_VALUE + THRESHOLD, 1024, 90, 180));
  else if (x <= POTENTIOMETER_BASE_VALUE - THRESHOLD)
      servo.write(map(x, 0, POTENTIOMETER_BASE_VALUE - THRESHOLD, 0, 90));
  else
      servo.write(90);
}