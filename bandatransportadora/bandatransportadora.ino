#include <LiquidCrystal.h>

#define lb1A 22
#define lb1B 24
#define lb1C 26
#define lb1D 28

#define lb2A 30
#define lb2B 32
#define lb2C 34
#define lb2D 36

#define CLAB1 38
#define CLAB2 40

#define LB1M 42
#define LB2M 44

//Motores Stepper banda transportadora
#define SI1 22
#define SI2 24
#define SI3 26
#define SI4 28

#define SD1 30
#define SD2 32
#define SD3 34
#define SD4 36
int direccion;

//pantalla lcd
const int rs = 43, en = 45, d4 = 46, d5 = 48, d6 = 50, d7 = 52;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  //Stepper 1
  pinMode(lb1A, OUTPUT);
  pinMode(lb1B, OUTPUT);
  pinMode(lb1C, OUTPUT);
  pinMode(lb1D, OUTPUT);

  //Stepper 2
  pinMode(lb2A, OUTPUT);
  pinMode(lb2B, OUTPUT);
  pinMode(lb2C, OUTPUT);
  pinMode(lb2D, OUTPUT);

  //Cargas
  pinMode(CLAB1, INPUT);
  pinMode(CLAB2, INPUT);

  //Mandar
  pinMode(LB1M, INPUT);
  pinMode(LB2M, INPUT);

  //LCD
  lcd.begin(16, 2);//Columnas y filas de la pantalla
  lcd.print("Proyecto 1");//Print a message to the LCD.
  lcd.setCursor(0, 1);
  lcd.print("Grupo 19 ");

  Serial.begin(9600);

  pinMode(SI1, OUTPUT);
  pinMode(SI2, OUTPUT);
  pinMode(SI3, OUTPUT);
  pinMode(SI4, OUTPUT);

  pinMode(SD1, OUTPUT);
  pinMode(SD2, OUTPUT);
  pinMode(SD3, OUTPUT);
  pinMode(SD4, OUTPUT);

  direccion = 0;
}

void loop() {

  if (digitalRead(LB1M) == 1) { //mandar de lab 1 a lab 2
    int carga1 = digitalRead(CLAB1);
    int carga2 = digitalRead(CLAB2);
    if (carga1 == 0 && carga2 == 0) {
      //avisar que no hay carga en lab1
      bandamsg(1, "1");
    } else if (carga1 == 0 && carga2 == 1) {
      //avisar que no hay carga en lab1
      bandamsg(1, "1");
    } else if (carga1 == 1 && carga2 == 0) {
      //mandar
      bandamsg(3, "no importa");
      direccion = 0;
      bool sigue = true;
      while (sigue) {
        if (digitalRead(CLAB1) == 0 ) {
          if (digitalRead(CLAB2) == 1) {
            sigue =  false;
          } else {
            sigue =  true;
          }
        } else {
          sigue =  true;
        }
        motores_banda();
      }
      bandamsg(4, "no importa");
    } else if (carga1 == 1 && carga2 == 1) {
      //avisar que hay carga en lab2
      bandamsg(2, "2");
    }
  } else if (digitalRead(LB2M) == 1) { //mandar de lab 2 a lab 1
    int carga1 = digitalRead(CLAB1);
    int carga2 = digitalRead(CLAB2);
    if (carga1 == 0 && carga2 == 0) {
      //avisar que no hay carga en lab2
      bandamsg(1, "2");
    } else if (carga1 == 0 && carga2 == 1) {
      //mandar
      bandamsg(5, "no importa");
      direccion = 1;
      bool sigue = true;
      while (sigue) {
        if (digitalRead(CLAB1) == 1 ) {
          if (digitalRead(CLAB2) == 0) {
            sigue =  false;
          } else {
            sigue =  true;
          }
        } else {
          sigue =  true;
        }
        motores_banda();
      }
      bandamsg(6, "no importa");
    } else if (carga1 == 1 && carga2 == 0) {
      //avisar que no hay carga en lab2
      bandamsg(1, "2");
    } else if (carga1 == 1 && carga2 == 1) {
      //avisar que hay carga en lab1
      bandamsg(2, "1");
    }
  } else {
    //no se hace nada
  }
}

void bandamsg(int c, String op) {
  switch (c) //donde opción es la variable a comparar
  {
    case 1: {
        lcd.setCursor(0, 0);
        lcd.print("");
        lcd.print("Poner muestra");
        lcd.setCursor(0, 1);
        lcd.print("");
        lcd.print("en la banda lab" + op);
        break;
      }
    case 2: {
        lcd.setCursor(0, 0);
        lcd.print("");
        lcd.print("Hay muestra");
        lcd.setCursor(0, 1);
        lcd.print("");
        lcd.print("en la banda lab" + op);
        break;
      }
    case 3: {
        lcd.setCursor(0, 0);
        lcd.print("");
        lcd.print("Corriendo hacia");
        lcd.setCursor(0, 1);
        lcd.print("");
        lcd.print("la izquierda    ");
        break;
      }
    case 4: {
        lcd.setCursor(0, 0);
        lcd.print("");
        lcd.print("La muestra llego");
        lcd.setCursor(0, 1);
        lcd.print("");
        lcd.print("a lab 2         ");
        break;
      }
    case 5: {
        lcd.setCursor(0, 0);
        lcd.print("");
        lcd.print("Corriendo hacia");
        lcd.setCursor(0, 1);
        lcd.print("");
        lcd.print("la derecha      ");
        break;
      }
    case 6: {
        lcd.setCursor(0, 0);
        lcd.print("");
        lcd.print("La muestra llego");
        lcd.setCursor(0, 1);
        lcd.print("");
        lcd.print("a lab 1         ");
        break;
      }
    default: break;
  }
}


void motores_banda() {

  if (!direccion) { //izquierda a derecha
    digitalWrite(SD1, LOW); digitalWrite(SD2, LOW); digitalWrite(SD3, LOW); digitalWrite(SD4, HIGH);
    digitalWrite(SI1, LOW); digitalWrite(SI2, LOW); digitalWrite(SI3, LOW); digitalWrite(SI4, HIGH);
    delay(150);
    digitalWrite(SD1, LOW); digitalWrite(SD2, LOW); digitalWrite(SD3, HIGH); digitalWrite(SD4, LOW);
    digitalWrite(SI1, LOW); digitalWrite(SI2, LOW); digitalWrite(SI3, HIGH); digitalWrite(SI4, LOW);
    delay(150);
    digitalWrite(SD1, LOW); digitalWrite(SD2, HIGH); digitalWrite(SD3, LOW); digitalWrite(SD4, LOW);
    digitalWrite(SI1, LOW); digitalWrite(SI2, HIGH); digitalWrite(SI3, LOW); digitalWrite(SI4, LOW);
    delay(150);
    digitalWrite(SD1, HIGH); digitalWrite(SD2, LOW); digitalWrite(SD3, LOW); digitalWrite(SD4, HIGH);
    digitalWrite(SI1, HIGH); digitalWrite(SI2, LOW); digitalWrite(SI3, LOW); digitalWrite(SI4, HIGH);
    delay(150);
  } else { //derecha a izquierda
    digitalWrite(SD1, HIGH); digitalWrite(SD2, LOW); digitalWrite(SD3, LOW); digitalWrite(SD4, HIGH);
    digitalWrite(SI1, HIGH); digitalWrite(SI2, LOW); digitalWrite(SI3, LOW); digitalWrite(SI4, HIGH);
    delay(150);
    digitalWrite(SD1, LOW); digitalWrite(SD2, HIGH); digitalWrite(SD3, LOW); digitalWrite(SD4, LOW);
    digitalWrite(SI1, LOW); digitalWrite(SI2, HIGH); digitalWrite(SI3, LOW); digitalWrite(SI4, LOW);
    delay(150);
    digitalWrite(SD1, LOW); digitalWrite(SD2, LOW); digitalWrite(SD3, HIGH); digitalWrite(SD4, LOW);
    digitalWrite(SI1, LOW); digitalWrite(SI2, LOW); digitalWrite(SI3, HIGH); digitalWrite(SI4, LOW);
    delay(150);
    digitalWrite(SD1, LOW); digitalWrite(SD2, LOW); digitalWrite(SD3, LOW); digitalWrite(SD4, HIGH);
    digitalWrite(SI1, LOW); digitalWrite(SI2, LOW); digitalWrite(SI3, LOW); digitalWrite(SI4, HIGH);
    delay(150);
  }
}
