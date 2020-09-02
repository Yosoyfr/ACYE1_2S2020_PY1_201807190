#include <Servo.h>
#define SERVO 23
#define BUZZER 25
Servo servoMotor;
void setup() {
  //pinMode(SERVO, OUTPUT);
  //digitalWrite(SERVO, LOW);
  //pinMode(BUZZER, OUTPUT);
  // Iniciamos el monitor serie para mostrar el resultado
  Serial.begin(9600);
  pinMode(25, OUTPUT);
  pinMode(27, OUTPUT);
  digitalWrite(25, HIGH);
  // Iniciamos el servo para que empiece a trabajar con el pin 9
  servoMotor.attach(SERVO);

  // Inicializamos al ángulo 0 el servomotor
  servoMotor.write(0);
}

void loop() {
  /*servoMotor.write(720);
    delay(3000);
    servoMotor.write(-720);

  */
  digitalWrite(27, HIGH);
  digitalWrite(25, LOW);
  for (int i = 0; i <= 359; i++)
  {
    // Desplazamos al ángulo correspondiente
    servoMotor.write(i);
    // Hacemos una pausa de 25ms
    delay(20);
  }
 //delay(3000);
  digitalWrite(25, HIGH);
  digitalWrite(27, LOW);
  // Para el sentido negativo
  for (int i = 359; i >= 0; i--)
  {
    // Desplazamos al ángulo correspondiente
    servoMotor.write(i);
    // Hacemos una pausa de 25ms
    delay(20);
  }

  //delay(3000);
}
