#include <Servo.h>
#define SERVO 9
#define BUZZER 29
#define BOTONSERVO 23
Servo servoMotor;
long tiempoSubida = 0;
long tiempoBuzzer=0;
bool sube = false;
bool regreso = true;
bool prenderBuzzer = false;
void setup() {

  Serial.begin(9600);
  pinMode(25, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(BOTONSERVO, INPUT);
  pinMode(BUZZER, OUTPUT);

  servoMotor.attach(SERVO);

  servoMotor.write(0);
}

void loop() {
  int leer = digitalRead(BOTONSERVO);
  if (!regreso && leer != 1) {
    sube = true;
  }

  if (leer == 1 && regreso) {
    for (int i = 0; i <= 359; i++)
    {
      servoMotor.write(i);
    }
    tiempoSubida = millis();

    regreso = false;
    digitalWrite(27, HIGH);
    digitalWrite(25, LOW);
  }

  if (millis() - tiempoSubida > 6000 || leer == 1 && sube) {

    for (int i = 359; i >= 0; i--)
    {
      servoMotor.write(i);
    }
    prenderBuzzer = true;
   regreso = false;
  }

  if(prenderBuzzer && servoMotor.read()==0){
     tiempoBuzzer = millis();
    digitalWrite(BUZZER, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(27, LOW);
    sube = false;
    regreso = false;
    prenderBuzzer = false;
  }

 if (millis() - tiempoBuzzer > 3000) {
    digitalWrite(BUZZER, LOW);
    digitalWrite(25, LOW);
    digitalWrite(27, LOW);
    tiempoSubida = 0;
    tiempoBuzzer = 0;
    regreso = true;
  }
}
