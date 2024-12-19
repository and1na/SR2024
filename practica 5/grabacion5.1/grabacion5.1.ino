
#include <Arduino.h>
#include <Servo.h>

/**
 * grabacion5.1 - Grupo L2-A5
 *
 * Alvaro Garcia Fernandez - UO289295
 * Daniel Andina Pailos - UO287791
 * Enol Rodriguez Hevia - UO287935
 */

#define RECORD_MODE 0
#define RUN_MODE 1

#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define BUTTON 7
#define THRESHOLD 50

#define ENDSTOP_1 3
#define ENDSTOP_2 2

#define SERVO 6

Servo servo;
const int POTENTIOMETER_BASE_VALUE = 1024 / 2;

void calibrate();
void record();
void run();
void interpreter(String *instructions, long size);
String serialRead();

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1);

  pinMode(JOYSTICK_X, INPUT);
  // pinMode(JOYSTICK_Y, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(ENDSTOP_1, INPUT);
  pinMode(ENDSTOP_2, INPUT);

  servo.attach(SERVO);
  // calibrate();
}

void loop() {
  long value = serialRead().toInt();

  switch (value) {
  case RECORD_MODE:
    record();
    break;
  case RUN_MODE:
    run();
    break;
  default:
    break;
  }
}

void record() {
  boolean finished = false;
  unsigned long lastTime = millis();

  while (!finished) {
    int x = analogRead(JOYSTICK_X);
    if (digitalRead(BUTTON) == LOW) {
      finished = true;
      Serial.println("FINISH");
    }
    if (x >= POTENTIOMETER_BASE_VALUE + THRESHOLD) {
      servo.write(180);
      while (x >= POTENTIOMETER_BASE_VALUE + THRESHOLD)
        x = analogRead(JOYSTICK_X);
      Serial.print("ADELANTE ");
      Serial.println(millis() - lastTime);
      lastTime = millis();
    } else if (x <= POTENTIOMETER_BASE_VALUE - THRESHOLD) {
      servo.write(0);
      while (x <= POTENTIOMETER_BASE_VALUE - THRESHOLD)
        x = analogRead(JOYSTICK_X);
      Serial.print("ATRAS ");
      Serial.println(millis() - lastTime);
      lastTime = millis();
    } else {
      servo.write(90);
      while (x <= POTENTIOMETER_BASE_VALUE + THRESHOLD &&
             x >= POTENTIOMETER_BASE_VALUE - THRESHOLD) {
        if (digitalRead(BUTTON) == LOW)
          break;
        x = analogRead(JOYSTICK_X);
      }
      Serial.print("PARAR ");
      Serial.println(millis() - lastTime);
      lastTime = millis();
    }
  }
}

void run() {
  // calibrate();
  long size = serialRead().toInt();
  if (size <= 0)
    return;

  Serial.println(size);

  String *values = (String *)malloc(size * sizeof(String));
  if (values == nullptr)
    return;

  for (long i = 0; i < size; i++) {
    String instruction = serialRead();
    Serial.println(instruction);
    values[i] = instruction;
    // delay(100);
  }
  // Serial.println("Received");
  interpreter(values, size);
  free(values);
}

void interpreter(String *instructions, long size) {
  for (long i = 0; i < size; i++) {
    String instruction = instructions[i];
    if (instruction.startsWith("ADELANTE")) {
      servo.write(0);
      unsigned long time =
          instruction.substring(instruction.lastIndexOf(' ')).toInt();
      delay(time);
    } else if (instruction.startsWith("ATRAS")) {
      servo.write(180);
      unsigned long time =
          instruction.substring(instruction.lastIndexOf(' ')).toInt();
      delay(time);
    } else if (instruction.startsWith("PARAR")) {
      servo.write(90);
      unsigned long time =
          instruction.substring(instruction.lastIndexOf(' ')).toInt();
      delay(time);
    } else if (instruction.startsWith("FINISH")) {
      servo.write(90);
      return;
    }
  }
}

void calibrate() {
  // Moves to one endstop
  servo.write(0);
  while (digitalRead(ENDSTOP_1) == HIGH) {
  }

  servo.write(90);
  const unsigned long time1 = millis();

  // Moves to the other onw
  servo.write(180);
  while (digitalRead(ENDSTOP_2) == HIGH) {
  }

  const unsigned long time2 = millis();
  servo.write(90);
  const unsigned long time = time2 - time1;

  // Moves to the middle
  servo.write(0);
  delay(time / 2);

  // Stops
  servo.write(90);
}

String serialRead() {
  String str = "";
  while (true) {
    if (Serial.available() > 0) {
      char rc = Serial.read();
      if (rc == '\n') {
        break;
      }
      str += rc;
    }
  }
  return str;
}
