
#include <Keypad.h>
#include <TM1637.h>

/**
 * Contador2.5 - Grupo L2-A5
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

#define ULTRASONIC_ECHO 13
#define ULTRASONIC_TRIGGER 12
#define ULTRASONIC_THRESHOLD 10

#define LDR A5
#define LDR_THRESHOLD 100

#define SCREEN_CLK 11
#define SCREEN_DIG 10

#define PASSWORD 9876
#define PASSWORD_LENGTH 4

TM1637 screen(SCREEN_CLK, SCREEN_DIG);
Keypad keypad(COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, ROW_1, ROW_2, ROW_3, ROW_4, false);

unsigned long pressedTime = 0;
int keysPressed = 0;
String password = "";
int usersCount = 0;
boolean isOutActivated = false;

void setup()
{
  Serial.begin(9600);
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

  pinMode(ULTRASONIC_ECHO, INPUT);
  pinMode(ULTRASONIC_TRIGGER, OUTPUT);

  pinMode(LDR, INPUT);

  keypad.begin();
  screen.init();
  screen.set(BRIGHT_TYPICAL);
  delay(1500);
  close();
  showNumber(0);
}

void loop()
{
  keypad.scan();
  char value = keypad.readAscii();

  // Check if the pressed key is a digit and adds it to the password
  if(isDigit(value))
  {
    password += value;
    keysPressed++;
  }

  // Check if the password is complete
  if(keysPressed == PASSWORD_LENGTH)
  {
    // Check if the password is correct and opens the door
    if(password.toInt() == PASSWORD)
    {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      delay(1000);
      pressedTime = millis();
    }
    // If the password is incorrect, blinks the green led
    else
      ledBlink();

    password = "";
    keysPressed = 0;
  }

  boolean someoneOut = calculateDistance() < ULTRASONIC_THRESHOLD;
  boolean someoneIn = analogRead(LDR) < LDR_THRESHOLD;
  boolean timeEnded = millis() - pressedTime >= 5000 && pressedTime != 0;

  if(someoneOut && pressedTime != 0)
    pressedTime = millis();
  if(someoneOut && !someoneIn)
    isOutActivated = true;

  if(pressedTime == 0 && someoneIn)
  {
    while (someoneIn)
    {
      open();
      someoneIn = analogRead(LDR) < LDR_THRESHOLD;
    }
    close();
  }
  else if(timeEnded && someoneOut && someoneIn) // Caso 2
    pressedTime = millis();
  else if((value == 'C' || timeEnded) && !someoneOut) // Caso 1
    pressedTime = 0;
  else if(!someoneOut && someoneIn)  // Caso 3
  {
    pressedTime = 0;
    if(isOutActivated)
      showNumber(++usersCount);
  }

  if (pressedTime != 0)
    open();
  else
    close();
}

float calculateDistance()
{
  digitalWrite(ULTRASONIC_TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(ULTRASONIC_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER, LOW);

  unsigned long duration = pulseIn(ULTRASONIC_ECHO, HIGH);

  float distance = (duration * 0.0343) / 2;

  return distance;
}

void open()
{
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
}

void close()
{
  isOutActivated = false;
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
}

void ledBlink()
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

void showNumber(int number)
{
  screen.clearDisplay();
  String n = String(number);
  unsigned int zeros = 4 - n.length();

  int8_t* output = new int8_t[4];

  for (int i = 4; i >= zeros; i--)
    output[i] = 0;

  for (unsigned int i = 0; i < n.length(); i++)
    output[i + zeros] = n[i] - '0';

  for(int i = 0; i < 4; i++)
    screen.display(i, output[i]);

  free(output);
}
