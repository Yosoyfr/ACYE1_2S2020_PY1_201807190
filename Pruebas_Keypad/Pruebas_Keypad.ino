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
struct user {
  char id[4];
  char password[8];
};

void setup()
{
  Serial.begin(9600);
  displayscreen();
  lcd.begin(16, 2);
  //clearEEPROM();

  int a = 0;
  EEPROM.put(0, a);

  char id[]= "1234";
  char contra[] = "12345678";
  

  user usuario1 = {"1234","12345678"};
  Serial.println(usuario1.id);
  Serial.println(usuario1.password);
  //addUser(usuario1);

  Serial.println(contadorUsuarios());

  int address = 2;
  for (int i = 0 ; i < contadorUsuarios() ; ++i) {
    user usuario;
    EEPROM.get(address, usuario);
    Serial.print("ID: ");
    Serial.print(usuario.id);
    Serial.print(" Contrasenia: ");
    Serial.println(usuario.password);

    address += sizeof(user);
  }
  /*
    for (int i = 0 ; i < EEPROM.length()  ; ++i) {
      Serial.print("EEPROM Data: ");
      Serial.print(i);
      Serial.print(" Value: ");

      Serial.println(EEPROM[i]);
    }
  */
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
void addUser(user usuario) {
  //Ingresamos el nuevo usuario
  int nUsuarios = contadorUsuarios();
  int index = nUsuarios * sizeof(user) + 2;
  EEPROM.put(index, usuario);
  //Aumentamos el contador de usuarios
  nUsuarios++;
  EEPROM.put(0, nUsuarios);
}


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
