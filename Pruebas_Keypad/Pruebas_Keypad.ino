#include <Keypad.h>
#include <LiquidCrystal.h>

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

void setup()
{
  displayscreen();
  Serial.begin(9600);
  lcd.begin(16, 2);
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
      if(verificarPass())
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

void incorrect()
{
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

void displayscreen()
{
  lcd.setCursor(0, 0);
  lcd.println("*BIENVENIDO*");
}
