#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <Servo.h>

#define SERVO 53
#define BUZZER 29
#define BOTONSERVO 23
#define BOTONCERRAR 33
#define LEDAMA 25
#define LEDROJ 27

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

//Motores Stepper banda transportadora
#define SI1 22
#define SI2 24
#define SI3 26
#define SI4 28

#define SD1 30
#define SD2 32
#define SD3 34
#define SD4 36

#define LEDLAB1 44
#define LEDLAB2 41
#define EE 39
#define SC 37

Servo servoMotor;
long tiempoSubida;
bool sube = false;
bool regreso = true;
bool ejecuto;
bool buzzer = false;

int direccion;

//pantalla lcd
const int rs = 43, en = 45, d4 = 46, d5 = 48, d6 = 50, d7 = 52;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String text;

byte candado[8] = {
  B01110,
  B10001,
  B10001,
  B10001,
  B11111,
  B11011,
  B11111,
  B11111
};

byte carita[8] = {
  B01010,
  B01010,
  B01010,
  B00000,
  B10001,
  B10001,
  B01110,
  B00000
};

byte ampersand[8] = {
  B01110,
  B10001,
  B01010,
  B00100,
  B01110,
  B10011,
  B01101,
  B00000

};

byte check[8] = {
  B00000,
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
  B00000,
  B00000
};

byte luces[4] = {false, false, false, false};

bool acceso = false;//var para saber si corremos el log in o las demás funciones

//----LOG IN

//matriz para el teclado
const byte rows = 4;
const byte cols = 3;
char key[rows][cols] = {
  {'7', '8', '9'},
  {'4', '5', '6'},
  {'1', '2', '3'},
  {'L', '0', 'E'}
};
byte rowPins[rows] = {5, 4, 3, 2};
byte colPins[cols] = {8, 7, 6};

Keypad keypad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);
char* password[8];
char* bPassword = "0106";//Esta variable será util para ver si el jefe autoriza la operación.
char uss[4] = {'\0', '\0', '\0', '\0'};
char pass[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
int currentposition = 0;
int invalidcount = 0;

typedef struct  {
  int id;
  long password;
} User;
//------------

void setup() {

  Serial.begin(9600);
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

  //LCD
  lcd.createChar(0, check);
  lcd.createChar(1, carita);
  lcd.createChar(2, candado);
  lcd.createChar(3, ampersand);

  lcd.begin(16, 2);//Columnas y filas de la pantalla
  lcd.print("Proyecto 1");//Print a message to the LCD.
  lcd.setCursor(0, 1);
  lcd.print("Grupo 19 ");

  pinMode(SI1, OUTPUT);
  pinMode(SI2, OUTPUT);
  pinMode(SI3, OUTPUT);
  pinMode(SI4, OUTPUT);

  pinMode(SD1, OUTPUT);
  pinMode(SD2, OUTPUT);
  pinMode(SD3, OUTPUT);
  pinMode(SD4, OUTPUT);
  pinMode(LEDAMA, OUTPUT);
  pinMode(LEDROJ, OUTPUT);


  pinMode(BOTONSERVO, INPUT);
  pinMode(BOTONCERRAR, INPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(LEDLAB1, OUTPUT);
  pinMode(LEDLAB2, OUTPUT);
  pinMode(EE, OUTPUT);
  pinMode(SC, OUTPUT);

  servoMotor.attach(SERVO);

  servoMotor.write(0);
  ejecuto = false;

  direccion = 0;

  //LOGIN
  //Configuracion de un boton reset system
  pinMode(13, INPUT);
  //Configuracion de pines para mostrar contraseña correcta o incorrecta
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  showUsers();


}

void loop() {
  if (!acceso) {
    looplogin();
  }
  else {
    while (Serial.available()) {
      delay(10);
      char c = Serial.read();
      text += c;
    }

    if (text.length() > 0) {
      //digitalRead(LB1M)
      if (text == "sendlab1") { //mandar de lab 1 a lab 2
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
        //digitalRead(LB2M)
      } else if (text == "sendlab2") { //mandar de lab 2 a lab 1
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
      } else if (text == "pon" || text == "poff") {
        porton(text);
      } else if (text == "luzlab1") {
        luces[0] = !luces[0];
        digitalWrite(LEDLAB1, luces[0]);
      } else if (text == "luzlab2") {
        luces[1] = !luces[1];
        digitalWrite(LEDLAB2, luces[1]);
      } else if (text == "luzEE") {
        luces[2] = !luces[2];
        digitalWrite(EE, luces[2]);
      } else if (text == "luzSC") {
        luces[3] = !luces[3];
        digitalWrite(SC, luces[3]);
      } else if (text == "luzallon") {
        for (int u = 0; u < 4; u++) {
          luces[u] = true;
        }

        digitalWrite(LEDLAB1, luces[0]);
        digitalWrite(LEDLAB2, luces[1]);
        digitalWrite(EE, luces[2]);
        digitalWrite(SC, luces[3]);
      } else if (text == "luzalloff") {
        for (int u = 0; u < 4; u++) {
          luces[u] = false;
        }

        digitalWrite(LEDLAB1, luces[0]);
        digitalWrite(LEDLAB2, luces[1]);
        digitalWrite(EE, luces[2]);
        digitalWrite(SC, luces[3]);

      } else if (text == "tempe") {
        int val = analogRead(A0);
        float mv = (val / 1024.0) * 5000;
        float temp = mv / 10;
        Serial.print(temp);
      } else {
        //no se hace nada
      }
      text = "";

    }
  }
}


void bandamsg(int c, String op) {
  switch (c) //donde opción es la variable a comparar
  {
    case 1: {
        lcd.setCursor(0, 0);
        lcd.print("Poner muestra   ");
        lcd.setCursor(0, 1);
        lcd.print("en la banda lab" + op);
        break;
      }
    case 2: {
        lcd.setCursor(0, 0);
        lcd.print("Hay muestra     ");
        lcd.setCursor(0, 1);
        lcd.print("en la banda lab" + op);
        break;
      }
    case 3: {
        lcd.setCursor(0, 0);
        lcd.print("Corriendo hacia");
        lcd.setCursor(0, 1);
        lcd.print("la izquierda    ");
        break;
      }
    case 4: {
        lcd.setCursor(0, 0);
        lcd.print("La muestra llego");
        lcd.setCursor(0, 1);
        lcd.print("a lab 2         ");
        break;
      }
    case 5: {
        lcd.setCursor(0, 0);
        lcd.print("Corriendo hacia");
        lcd.setCursor(0, 1);
        lcd.print("la derecha      ");
        break;
      }
    case 6: {
        lcd.setCursor(0, 0);
        lcd.print("La muestra llego");
        lcd.setCursor(0, 1);
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


void porton(String text) {
  int leer = digitalRead(BOTONSERVO);

  if (text == "pon") {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.write((byte)0);//check
    lcd.write((byte)3);//ampersand
    lcd.print("ControlPorton");
    lcd.write((byte)3);
    lcd.setCursor(0, 1);
    lcd.write((byte)2);//candado
    lcd.write((byte)1);//carita
    lcd.print("Abriendo");
    lcd.write((byte)1);//carita

    for (int i = 0; i <= 359; i++)
    {
      servoMotor.write(i);
    }
    ejecuto = true;

    digitalWrite(LEDROJ, HIGH);
    digitalWrite(LEDAMA, LOW);
  }

  if (ejecuto) {
    bool boton = false;
    tiempoSubida = millis();

    while (millis() - tiempoSubida < 10000 &&  text != "poff") {
      text = "";
      while (Serial.available()) {
        delay(10);
        char c = Serial.read();
        text += c;
      }
      if (boton == 1) {
        break;
      }
    }

    for (int i = 359; i >= 0; i--)
    {
      servoMotor.write(i);
    }
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.write((byte)0);//check
    lcd.write((byte)3);//ampersand
    lcd.print("ControlPorton");
    lcd.write((byte)3);
    lcd.setCursor(0, 1);
    lcd.write((byte)2);//candado
    lcd.write((byte)1);//carita
    lcd.print("Cerrando");
    lcd.write((byte)1);//carita
    delay(3000);
    long tiempoBuzzer = millis();
    ejecuto = false;
    while (millis() - tiempoBuzzer < 3000) {
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LEDAMA, HIGH);
      digitalWrite(LEDROJ, LOW);
    }
    digitalWrite(BUZZER, LOW);
    digitalWrite(LEDAMA, LOW);
    digitalWrite(LEDROJ, LOW);
  }
}

//-----------------------------------------------------------------LOG IN------------------------------------------------------------------------

bool looplogin() {
  if ( currentposition == 0)
  {
    displayscreen();
  }
  int l ;
  char code = keypad.getKey();
  if (code != NO_KEY)
  {
    if (code != 'E' )
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("USUARIO:");
      lcd.setCursor(7, 1);
      lcd.print(" ");
      lcd.setCursor(7, 1);
      uss[currentposition] = code;
      for (l = 0; l <= currentposition; ++l)
      {
        lcd.print(uss[l]);
      }
      ++currentposition;
    }
    else
    {
      // Área para verificar si pertenece a la matriz.
      if (strncmp(uss, "0000", 4 ) == 0)
      {
        acceso = false;
        addNewUser();
      }
      else if (verificarPass())
      {
        acceso = true;
        for (int u = 0; u < 4; u++) {
          luces[u] = true;
        }

        digitalWrite(LEDLAB1, luces[0]);
        digitalWrite(LEDLAB2, luces[1]);
        digitalWrite(EE, luces[2]);
        digitalWrite(SC, luces[3]);
        correct();
        invalidcount = 0;
      }
      else
      {
        acceso = false;
        ++invalidcount;
        incorrect();
      }
      currentposition = 0;
      memset (uss, '\0', 4);
      memset (pass, '\0', 8);
    }
  }
}


void clearEEPROM()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    if (EEPROM.read(i) != 0)                    //skip already "empty" addresses
    {
      EEPROM.write(i, 0);                       //write 0 to address i
    }
  }                          //reset address counter
}

void incorrect()
{
  //Imprimir mensaje de pass incorrecto
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("PASS INCORRECTO");
  delay(2000);

  // Verificar si la cuenta es más de 4 bloquear.
  if (invalidcount == 4)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.println("Sistema bloqueado,");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.println("avise al gerente.");
    //Generar tono en buzzer por 5 segundos.
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    delay(5000);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    //Pedir autorización del gerente.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("GERENTE:");
    lcd.setCursor(0, 1);
    int confirmCount = 0;
    char bossPass[4];
    while (true)
    {
      char code = keypad.getKey();
      if (code != NO_KEY)
      {
        if (code != 'E' && confirmCount < 4)
        {
          bossPass[confirmCount] = code;
          lcd.print(code);
          confirmCount++;
        }
        else
        {
          if (strncmp(bPassword, bossPass, 4) == 0)
          {
            // Desbloquear el sistema.
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("DESBLOQUEADO    ");
            delay(2000);
            invalidcount = 0;
            break;
          }
          else
          {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("INCORRECTO      ");
            delay(2000);

            //Pedir autorización del gerente de nuevo.
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("GERENTE:");
            lcd.setCursor(0, 1);
            memset(bossPass, '\0', 4);
            confirmCount = 0;
          }
        }
      }
    }
  }
  lcd.clear();
}

void correct()
{
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("PASS");
  lcd.setCursor(6, 0);
  lcd.print("CORRECTO");
  lcd.setCursor(15, 1);
  lcd.println(" ");
  digitalWrite(12, HIGH);
  digitalWrite(10, HIGH);
  delay(2000);
  lcd.clear();
  displayscreen();
  digitalWrite(12, LOW);
  digitalWrite(10, LOW);
}

/*
   Metodo que recupera el numero de usuarios registrados en el sistema
   de la memeria EEPROM del arduino, la cual serian las primeras dos posiciones
*/
int contadorUsuarios() {
  int nUsuarios;
  EEPROM.get(0, nUsuarios);
  return nUsuarios;
}

/*
   Metodo para insertar un nuevo usuario del sistema a la memoria EEPROM
*/
void addUser(User usuario) {
  //Ingresamos el nuevo usuario
  int nUsuarios = contadorUsuarios();
  int index = nUsuarios * sizeof(User) + 2;
  EEPROM.put(index, usuario);
  //Aumentamos el contador de usuarios
  nUsuarios++;
  EEPROM.put(0, nUsuarios);
}

void displayscreen()
{
  lcd.setCursor(0, 0);
  lcd.println("*BIENVENIDO*    ");
}

bool verificarPass() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("PASSWORD:       ");
  lcd.setCursor(0, 1);
  int passC = 0;
  while (true)
  {
    char code = keypad.getKey();
    if (code != NO_KEY)
    {
      if (code != 'E')
      {
        pass[passC++] = code;
        lcd.print(code);
      }
      else if (passC >= 8)
      {
        break;
      }
      else
      {
        break;
      }
    }
  }
  delay(2000);
  return usersLogin(String(uss).toInt(), atol(pass));
}

/*
   Metodo para verificar el log correcto de un usuario
*/
bool usersLogin(int id, long password) {
  int address = 2;
  for (int i = 0 ; i < contadorUsuarios() ; ++i) {
    User user;
    EEPROM.get(address, user);
    if (user.id == id && user.password == password) {
      return true;
    }
    address += sizeof(User);
  }
  return false;
}

/*
   Metodo para mostrar todos los usuarios en el sistema guardados en la EEPROM
*/
void showUsers() {
  int address = 2;
  for (int i = 0 ; i < contadorUsuarios() ; ++i) {
    User user;
    EEPROM.get(address, user);
    address += sizeof(User);
  }
}

void addNewUser()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PASS:           ");
  lcd.setCursor(0, 1);
  char newPass[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
  char newConfirm[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
  int count = 0;
  while (true)
  {
    char code = keypad.getKey();
    if (code != NO_KEY)
    {
      if (code == 'E')
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CONFIRMAR:      ");
        lcd.setCursor(0, 1);

        int confirmCount = 0;
        while (confirmCount < 8)
        {
          code = keypad.getKey();
          if (code != NO_KEY)
          {
            if (code == 'E')break;
            newConfirm[confirmCount] = code;
            lcd.print(code);
            confirmCount++;
          }
        }
        delay(1000);
        bool match = (strncmp(newPass, newConfirm, 8) == 0);
        if (match)
        {
          //Pedir autorización del gerente.
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("GERENTE:        ");
          lcd.setCursor(0, 1);
          confirmCount = 0;
          char bossPass[4];
          while (confirmCount < 4)
          {
            code = keypad.getKey();
            if (code != NO_KEY)
            {
              bossPass[confirmCount] = code;
              lcd.print(code);
              confirmCount++;
            }
          }
          bool autorized = (strncmp(bossPass, bPassword, 4) == 0);

          if (autorized)
          {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("AUTORIZADA      ");
            delay(2000);

            //Imprimir nuevo codigo de usuario
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("USUARIO:        ");
            lcd.setCursor(0, 1);
            int newCode = contadorUsuarios();
            lcd.println(String(newCode));
            delay(2000);

            //Imprimir su contraseña
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("PASS:");
            lcd.setCursor(0, 1);
            lcd.println(String(newPass));
            delay(2000);

            //Agregarlo a la EEPROM
            User newUser = {(int)newCode, atol(newPass)};
            addUser(newUser);
          }
          else
          {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("NO AUTORIZADA");
            delay(2000);
          }
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("PASS NO COINCIDE");
          delay(2000);
        }
        //Romper el ciclo
        break;
      }
      else if (count >= 8)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("PASS MAX DE 8");
        delay(2000);
        break;
      }
      else
      {
        //Almacenar en newPass el caracter deseado.
        newPass[count] = code;
        count++;
        //Imprimir la contraseña.
        lcd.print(code);
      }
    }
  }
  lcd.clear();
}
