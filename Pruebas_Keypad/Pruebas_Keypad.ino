#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

int pos = 0; // LCD Connections
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
const byte rows = 4;
const byte cols = 3;
char key[rows][cols] = {
  {'7', '8', '9'},
  {'4', '5', '6'},
  {'1', '2', '3'},
  {'*', '0', 'E'}
};
byte rowPins[rows] = {5, 4, 3, 2};
byte colPins[cols] = {8, 7, 6};
Keypad keypad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);
char* password = "4567";
char pass[4] = {'\0', '\0', '\0', '\0'};

int currentposition = 0;
int invalidcount = 12;

/*
   Struct que representa un usuario en el sistema
*/
typedef struct  {
  int id;  
  long password;   
} User;

void setup()
{
  Serial.begin(9600);
  displayscreen();
  lcd.begin(16, 2);
  //clearEEPROM();

  int a = 0;
  EEPROM.put(0, a);

  User usuario1 ={4444, 88888888};
  User usuario2 ={1234, 12345678};
  EEPROM.put(0,usuario1);
  EEPROM.put(8,usuario2);
  User us1;
  User us2;
  EEPROM.get(0, us1);
  Serial.println(us1.id);
  Serial.println(us1.password);//NICE
  Serial.println(sizeof(us1));
  Serial.println("......Cisco hueco......");

  EEPROM.get(8, us2);
  Serial.println(us2.id);
  Serial.println(us2.password);//NICE
  Serial.println(sizeof(us2));
  //addUser(usuario1);

  
  //Serial.println(contadorUsuarios());

//  int address = 2;
//  for (int i = 0 ; i < contadorUsuarios() ; ++i) {
//    User usuario;
//    EEPROM.get(address, usuario);
//    Serial.print("ID: ");
//    Serial.print(usuario.color);
//    Serial.print(" Contrasenia: ");
//    Serial.println(usuario.name);
//
//    address += sizeof(User);
//  }
}

void loop()
{
  if ( currentposition == 0)
  {
    displayscreen();
  }
  int l ;
  char code = keypad.getKey();
  if (code != NO_KEY)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PASSWORD:");
    lcd.setCursor(7, 1);
    lcd.print(" ");
    lcd.setCursor(7, 1);
    pass[currentposition] = code;
    for (l = 0; l <= currentposition; ++l)
    {
      lcd.print(pass[l]);
    }
    ++currentposition;
    if (currentposition == 4)
    {
      // Área para verificar si pertenece a la matriz.
      if (verificarPass())
      {
        correct();
        invalidcount = 0;
      }
      else
      {
        ++invalidcount;
        incorrect();
      }
      currentposition = 0;
    }
  }
}

void clearEEPROM()
{
  Serial.println("EEPROM");
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    if (EEPROM.read(i) != 0)                    //skip already "empty" addresses
    {
      EEPROM.write(i, 0);                       //write 0 to address i
    }
  }
  Serial.println("EEPROM erased");                            //reset address counter
}

void incorrect()
{
  memset(pass, '\0', 4);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("PASS");
  lcd.setCursor(6, 0);
  lcd.print("INCORRECTO");
  lcd.setCursor(15, 1);
  lcd.println(" ");
  delay(200);
  lcd.clear();
  displayscreen();
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
  delay(200);
  lcd.clear();
  displayscreen();
}

///*
//   Metodo que recupera el numero de usuarios registrados en el sistema
//   de la memeria EEPROM del arduino, la cual serian las primeras dos posiciones
//*/
//int contadorUsuarios() {
//  int nUsuarios;
//  EEPROM.get(0, nUsuarios);
//  return nUsuarios;
//}
///*
//   Metodo para insertar un nuevo usuario del sistema a la memoria EEPROM
//*/
//void addUser(User usuario) {
//  //Ingresamos el nuevo usuario
//  int nUsuarios = contadorUsuarios();
//  int index = nUsuarios * sizeof(User) + 2;
//  EEPROM.put(index, usuario);
//  //Aumentamos el contador de usuarios
//  nUsuarios++;
//  EEPROM.put(0, nUsuarios);
//}


void displayscreen()
{
  lcd.setCursor(0, 0);
  lcd.println("*BIENVENIDO*");
}

bool verificarPass() {
  for (int i = 0; i < 4; i++) {
    if (pass[i] != password[i]) {
      return false;
    }
  }
  return true;
}
