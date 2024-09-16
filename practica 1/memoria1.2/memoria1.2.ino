
/**
 * Memoria1.2 - Grupo L2-A5
 * 
 * Alvaro Garcia Fernandez - UO289295
 * Daniel Andina Pailos - UO287791
 * Enol Rodriguez Hevia - UO287935
 */

#define ROJO 0
#define VERDE 1

#define MAX_SEQUENCE 3

#define PULSADOR_VERDE 4
#define PULSADOR_ROJO 5
#define LED_ROJO 6
#define LED_VERDE 7

int values[3];

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(PULSADOR_VERDE, INPUT);
  pinMode(PULSADOR_ROJO, INPUT);

  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
}

void loop() {
	for(int i = 0; i < MAX_SEQUENCE; i++) {
    values[i] = random(0, 2);
	}

  for(int i = 0; i < MAX_SEQUENCE; i++) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, LOW);
    delay(100);
    
    switch (values[i]) {
      case ROJO:
        digitalWrite(LED_ROJO, HIGH);
        break;
      case VERDE:
        digitalWrite(LED_VERDE, HIGH);
        break;
    }
  }

  delay(2000);
}
