#include <Servo.h>
#define SERVO 9
#define BUZZER 29
#define BOTONSERVO 23
#define BOTONCERRAR 33
Servo servoMotor;
long tiempoSubida;
bool sube = false;
bool regreso = true;
bool ejecuto;
bool buzzer = false;
void setup() {

  Serial.begin(9600);
  pinMode(25, OUTPUT);
  pinMode(27, OUTPUT);

  pinMode(BOTONSERVO, INPUT);
  pinMode(BOTONCERRAR, INPUT);
  pinMode(BUZZER, OUTPUT);

  servoMotor.attach(SERVO);

  servoMotor.write(0);
  ejecuto = false;
}

void loop() {
  int leer = digitalRead(BOTONSERVO);

  if (leer == 1) {
    for (int i = 0; i <= 359; i++)
    {
      servoMotor.write(i);
    }
    ejecuto = true;

    digitalWrite(27, HIGH);
    digitalWrite(25, LOW);
  }

  if (ejecuto) {
    bool boton = false;
    tiempoSubida = millis();

    while (millis() - tiempoSubida < 7000 &&  boton == false) {
      boton = digitalRead(BOTONCERRAR);
      if (boton == 1) {
        break;
      }
    }


    for (int i = 359; i >= 0; i--)
    {
      servoMotor.write(i);
    }
    delay(3000);
    long tiempoBuzzer = millis();
    ejecuto = false;
    while (millis() - tiempoBuzzer < 3000) {
      digitalWrite(BUZZER, HIGH);
      digitalWrite(25, HIGH);
      digitalWrite(27, LOW);
    }
    digitalWrite(BUZZER, LOW);
    digitalWrite(25, LOW);
    digitalWrite(27, LOW);
  }
}
