
#include <Servo.h>

/**
 * Automatico4.4 - Grupo L2-A5
 *
 * Alvaro Garcia Fernandez - UO289295
 * Daniel Andina Pailos - UO287791
 * Enol Rodriguez Hevia - UO287935
 */

#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define BUTTON 7
#define THRESHOLD 50

#define ENDSTOP_1 3
#define ENDSTOP_2 2

#define SERVO 6

Servo servo;
const int POTENTIOMETER_BASE_VALUE = 1024/2;
bool manualMode = false;
bool pressed = false;
bool isGoingToRight = true;

void setup()
{
    Serial.begin(9600);
    pinMode(JOYSTICK_X, INPUT);
    pinMode(JOYSTICK_Y, INPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    pinMode(ENDSTOP_1, INPUT);
    pinMode(ENDSTOP_2, INPUT);

    servo.attach(SERVO);
}

void loop()
{
    const int x = analogRead(JOYSTICK_X);
    if(!pressed && digitalRead(BUTTON) == LOW)
        pressed = true;

    if(pressed && digitalRead(BUTTON) == HIGH)
    {
        manualMode = !manualMode;
        pressed = false;
    }

    if(digitalRead(ENDSTOP_1) == LOW || digitalRead(ENDSTOP_2) == LOW)
    {
        servo.write(90);
        if(!manualMode)
        {
            isGoingToRight = !isGoingToRight;
            autoMove();
            delay(10);
        }
        return;
    }

    if(manualMode)
        manualMove(x);
    else
        autoMove();
}

void manualMove(int x)
{
    if (x >= POTENTIOMETER_BASE_VALUE + THRESHOLD)
        servo.write(map(x, POTENTIOMETER_BASE_VALUE + THRESHOLD, 1024, 90, 180));
    else if (x <= POTENTIOMETER_BASE_VALUE - THRESHOLD)
        servo.write(map(x, 0, POTENTIOMETER_BASE_VALUE - THRESHOLD, 0, 90));
    else
        servo.write(90);
}

void autoMove()
{
    if(isGoingToRight)
        servo.write(180);
    else
        servo.write(0);
}