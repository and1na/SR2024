
#include <Keypad.h>

/**
 * Abrir2.1 - Grupo L2-A5
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

Keypad keypad(COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, ROW_1, ROW_2, ROW_3, ROW_4, false);

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

  close();
}

void loop()
{
  char value = keypad.readBlocking();

  if(Keypad::convertToAscii(value) == 'A')
  {
    open();
    delay(5000);
  }
  close();
}

void open()
{
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
}

void close()
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
}
