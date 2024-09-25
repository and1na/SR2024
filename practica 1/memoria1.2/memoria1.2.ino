
/**
 * Memoria1.2 - Grupo L2-A5
 *
 * Alvaro Garcia Fernandez - UO289295
 * Daniel Andina Pailos - UO287791
 * Enol Rodriguez Hevia - UO287935
 */

#define ROJO 0
#define VERDE 1

#define PULSADOR_VERDE 2
#define PULSADOR_ROJO 4
#define LED_ROJO 7
#define LED_VERDE 8

// Numero maximo de turnos por partido
#define MAX_TURN 3
// Numero de colores en la secuencia inicial
#define INITIAL_SEQUENCE_LENGTH 3
// Numero maximo de colores en la secuencia
const int MAX_SEQUENCE_LENGTH = MAX_TURN + INITIAL_SEQUENCE_LENGTH - 1;

// Turno actual
int currentTurn = 0;
// Comprueba si se ha iniciado el juego
boolean started = false;

// Array con la secuencia de colores
int values[MAX_SEQUENCE_LENGTH];

// Funcion para resetear la placa de arduino
void (* reset) (void) = 0;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(PULSADOR_VERDE, INPUT);
  pinMode(PULSADOR_ROJO, INPUT);

  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  // Se inicializan los valores de la secuencia a -1
  for(int i = 0; i < MAX_SEQUENCE_LENGTH; i++)
  {
    values[i] = -1;
  }
}

void loop() {
  // Si se ha llegado al maximo numero de turnos, el jugador habra ganado
  if(currentTurn == MAX_TURN)
  {
    winSequence();
    reset();
  }

  if(started && digitalRead(PULSADOR_VERDE) == LOW && digitalRead(PULSADOR_ROJO) == LOW) {
    // Genera la secuencia para el turno actual
    generateSequence(currentTurn);
    // Muestra dicha secuencia
    displaySequence();
    // Espera por la interaccion del usuario
    handleUserInteraction();
    // Se continua al siguiente turno
    currentTurn++;

    for(int i = 0; i < MAX_SEQUENCE_LENGTH; i++)
    {
      if(values[i] == -1) break;
      Serial.println(values[i] == ROJO ? "rojo" : "verde");
    }
  }

  if(!started && digitalRead(PULSADOR_VERDE) == HIGH && digitalRead(PULSADOR_ROJO) == HIGH) {
    started = true;
  }
}

void displaySequence()
{
  for (int i = 0; i < MAX_SEQUENCE_LENGTH; i++)
  {
    // Si el valor de la secuencia es -1, se ha terminado la secuencia para el turno actual
    if (values[i] == -1) break;

    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_VERDE, LOW);
    delay(1000);

    switch (values[i])
    {
    case ROJO:
      digitalWrite(LED_ROJO, HIGH);
      break;
    case VERDE:
      digitalWrite(LED_VERDE, HIGH);
      break;
    }
    delay(1000);
  }
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
}

void generateSequence(int turn)
{
  for (int i = 0; i < INITIAL_SEQUENCE_LENGTH + currentTurn; i++)
  {
    // Si ya habia un numero asignado permanece
    if(values[i] != -1) continue;
    // En caso contrario se añaden numeros a la secuencia anterior
    if(i < INITIAL_SEQUENCE_LENGTH + turn) values[i] = random(ROJO, VERDE + 1);
  }
}

void handleUserInteraction()
{
  // Boton presionado por el usuario
  int buttonPressed = -1;
  // Color de la secuencia actual
  int sequenceIndex = 0;

  while (sequenceIndex < INITIAL_SEQUENCE_LENGTH + currentTurn)
  {
    if(buttonPressed != -1 && digitalRead(PULSADOR_ROJO) == LOW && digitalRead(PULSADOR_VERDE) == LOW) {
      // Se comprueba si el boton pulsado es el correcto
      if(values[sequenceIndex] == buttonPressed)
      {
        sequenceIndex++;
        buttonPressed = -1;
      }
      // Si no es correcta, se pierde y se reinicia el juego
      else
      {
        loseSequence();
        reset();
      }
    }

    // Se guarda el boton presionado por el usuario
    if(buttonPressed == -1 && (digitalRead(PULSADOR_ROJO) == HIGH || digitalRead(PULSADOR_VERDE) == HIGH)) {
      buttonPressed = digitalRead(PULSADOR_ROJO) == HIGH ? ROJO : VERDE;
      const int led = buttonPressed == ROJO ? LED_ROJO : LED_VERDE;
      digitalWrite(led, HIGH);
      delay(1000);
      digitalWrite(led, LOW);
    }
  }
}

void loseSequence()
{
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);

  for(int i = 0; i < 3; i++)
  {
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_VERDE, LOW);
    delay(500);
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_VERDE, HIGH);
    delay(500);
  }

  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
}

void winSequence()
{
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);

  for(int i = 0; i < 3; i++)
  {
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_VERDE, HIGH);
    delay(500);
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_VERDE, LOW);
    delay(500);
  }

  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
}
