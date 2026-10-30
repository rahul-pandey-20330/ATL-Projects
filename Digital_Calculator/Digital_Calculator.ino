
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <math.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys),
                       rowPins, colPins, ROWS, COLS);

// Raw detected key -> intended physical key
char convertKey(char raw) {
  switch (raw) {
    case 'D': return '1';
    case 'C': return '2';
    case 'B': return '3';
    case '#': return '4';
    case '9': return '5';
    case '6': return '6';
    case '0': return '7';
    case '8': return '8';
    case '5': return '9';
    case '7': return '0';
    case '*': return '*';
    case '4': return '#';
    case 'A': return 'A';
    case '3': return 'B';
    case '2': return 'C';
    case '1': return 'D';
    default:  return '\0';
  }
}

char firstNum[10] = "";
char secondNum[10] = "";
byte len1 = 0;
byte len2 = 0;
char op = '\0';
bool secondMode = false;
bool resultShown = false;

void showInput() {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (len1 == 0) {
    lcd.print("Enter first no.");
  } else {
    lcd.print(firstNum);

    if (op != '\0') {
      lcd.print(' ');
      if (op == 'A') lcd.print('+');
      if (op == 'B') lcd.print('-');
      if (op == 'C') lcd.print('x');
      if (op == 'D') lcd.print('/');
      lcd.print(' ');
      lcd.print(secondNum);
    }
  }

  lcd.setCursor(0, 1);
  lcd.print("A+ B- Cx D/");
}

void clearCalc() {
  len1 = 0;
  len2 = 0;
  firstNum[0] = '\0';
  secondNum[0] = '\0';
  op = '\0';
  secondMode = false;
  resultShown = false;
  showInput();
}

void addDigit(char key) {
  if (!secondMode && len1 < 9) {
    firstNum[len1++] = key;
    firstNum[len1] = '\0';
  } else if (secondMode && len2 < 9) {
    secondNum[len2++] = key;
    secondNum[len2] = '\0';
  }
  showInput();
}

void calculate() {
  if (len1 == 0 || len2 == 0 || op == '\0') {
    lcd.clear();
    lcd.print("Enter both nums");
    delay(1000);
    showInput();
    return;
  }

  double a = atof(firstNum);
  double b = atof(secondNum);
  double ans = 0;

  switch (op) {
    case 'A': ans = a + b; break;
    case 'B': ans = a - b; break;
    case 'C': ans = a * b; break;
    case 'D':
      if (b == 0) {
        lcd.clear();
        lcd.print("Math Error");
        lcd.setCursor(0, 1);
        lcd.print("Divide by zero");
        resultShown = true;
        return;
      }
      ans = a / b;
      break;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(firstNum);
  if (op == 'A') lcd.print('+');
  if (op == 'B') lcd.print('-');
  if (op == 'C') lcd.print('x');
  if (op == 'D') lcd.print('/');
  lcd.print(secondNum);

  lcd.setCursor(0, 1);
  if (isnan(ans) || isinf(ans)) {
    lcd.print("Math Error");
  } else {
    lcd.print("=");
    lcd.print(ans, 4);
  }

  resultShown = true;
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Digital Calc");
  delay(1000);
  clearCalc();
}

void loop() {
  char raw = keypad.getKey();
  if (!raw) return;

  char key = convertKey(raw);
  if (key == '\0') return;

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print(" -> Key: ");
  Serial.println(key);

  if (key == '*') {
    clearCalc();
    return;
  }

  if (resultShown) return;

  if (key >= '0' && key <= '9') {
    addDigit(key);
  } else if (key >= 'A' && key <= 'D') {
    if (len1 > 0 && op == '\0') {
      op = key;
      secondMode = true;
      showInput();
    }
  } else if (key == '#') {
    calculate();
  }
}
