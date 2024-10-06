
#include <Keypad.h>

/**
 * Contrasena2.3 - Grupo L2-A5
 *
 * Alvaro Garcia Fernandez - UO289295
 * Daniel Andina Pailos - UO287791
 * Enol Rodriguez Hevia - UO287935
 */

#define RED_LED 7
#define GREEN_LED 8

#define COLUMN_1 2
#define COLUMN_2 3
#define COLUMN_3 4
#define COLUMN_4 5

#define ROW_1 A0
#define ROW_2 A1
#define ROW_3 A2
#define ROW_4 A3

#define PASSWORD 9876
#define PASSWORD_LENGTH 4

Keypad keypad(COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, ROW_1, ROW_2, ROW_3, ROW_4, false);

unsigned long pressedTime = 0;
int keysPressed = 0;
String password = String();

void setup()
{
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    pinMode(COLUMN_1, INPUT);
    pinMode(COLUMN_2, INPUT);
    pinMode(COLUMN_3, INPUT);
    pinMode(COLUMN_4, INPUT);

    pinMode(ROW_1, INPUT);
    pinMode(ROW_2, INPUT);
    pinMode(ROW_3, INPUT);
    pinMode(ROW_4, INPUT);

    keypad.begin();

    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
}

void loop()
{
    keypad.scan();
    char value = keypad.readAscii();

    if(isDigit(value))
    {
        password += value;
        keysPressed++;
    }

    if(keysPressed == PASSWORD_LENGTH)
    {
        if(password.toInt() == PASSWORD)
        {
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            delay(1000);
            pressedTime = millis();
        }
        else
        {
            digitalWrite(RED_LED, LOW);
            for(int i = 0; i < 3; i++)
            {
                digitalWrite(GREEN_LED, LOW);
                delay(1000);
                digitalWrite(GREEN_LED, HIGH);
                delay(1000);
            }
        }

        password = "";
        keysPressed = 0;
    }

    if(value == 'C' || (millis() - pressedTime >= 5000 && pressedTime != 0))
    {
        pressedTime = 0;
    }

    if (pressedTime != 0)
    {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
    }
    else
    {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
    }
}
