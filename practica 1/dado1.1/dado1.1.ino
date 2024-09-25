
/**
 * Dado1.1 - Grupo L2-A5
 * 
 * Alvaro Garcia Fernandez - UO289295
 * Daniel Andina Pailos - UO287791
 * Enol Rodriguez Hevia - UO287935
 */

#define PULSADOR 3
#define LED1 4
#define LED2 5
#define LED3 6

boolean pulsado = false;

void setup() {
  pinMode(PULSADOR, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  Serial.begin(9600);
  randomSeed(analogRead(0));

  Serial.println("[+] Inicio");
}

void loop() {
  if(pulsado && digitalRead(PULSADOR) == LOW) {
    int value = random(1, 4);

    Serial.println(value);
    
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);

    switch (value) {
      case 3:
        digitalWrite(LED3, HIGH);
      case 2:
        digitalWrite(LED2, HIGH);
      case 1:
        digitalWrite(LED1, HIGH);
    }
    
    pulsado = false;
  }

  if(digitalRead(PULSADOR) == HIGH && !pulsado) {
    pulsado = true;  
  }
}
