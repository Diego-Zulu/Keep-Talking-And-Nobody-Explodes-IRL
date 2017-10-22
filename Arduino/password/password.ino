
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <TrueRandom.h>

#define I2C_ADDR    0x3F
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define BACK_BTN 2
#define FORW_BTN 3
#define ACCEPT_BTN 6
#define CODE_LED 4
#define BIG_LED 5
#define FREQUENCIES 16

#define UP_0_BTN 2
#define DOWN_0_BTN 7
#define UP_1_BTN 3
#define DOWN_1_BTN 8
#define UP_2_BTN 4
#define DOWN_2_BTN 9
#define UP_3_BTN 5
#define DOWN_3_BTN 10
#define UP_4_BTN 6
#define DOWN_4_BTN 11
#define VICTORY_LED 12
#define INPUT_ANSWER 13

#define BTN_PRESS_DELAY 300
#define char_amount 5

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

String password = "ABOUT";

char posibilities[char_amount][char_amount];

int positions[char_amount];

bool won = false;


void setup() {

  Serial.begin(9600);


  Serial.println();

  for (int i = 0; i < char_amount; i++) {
    positions[i] = 0;
  }

  for (int i = 0; i < char_amount; i++) {
    posibilities[i][0] = password[i];
    for (int j = 1; j < char_amount; j++) {
      char cch = 'A' + TrueRandom.random(0,26);
      posibilities[i][j] = cch;
    }
    shuffle(posibilities[i]);
  }

  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  pinMode(UP_0_BTN, INPUT_PULLUP);
  pinMode(DOWN_0_BTN, INPUT_PULLUP);
  pinMode(UP_1_BTN, INPUT_PULLUP);
  pinMode(DOWN_1_BTN, INPUT_PULLUP);
  pinMode(UP_2_BTN, INPUT_PULLUP);
  pinMode(DOWN_2_BTN, INPUT_PULLUP);
  pinMode(UP_3_BTN, INPUT_PULLUP);
  pinMode(DOWN_3_BTN, INPUT_PULLUP);
  pinMode(UP_4_BTN, INPUT_PULLUP);
  pinMode(DOWN_4_BTN, INPUT_PULLUP);
  pinMode(DOWN_4_BTN, INPUT_PULLUP);

  pinMode(VICTORY_LED, OUTPUT);

  pinMode(INPUT_ANSWER, INPUT_PULLUP);

  printToLcd();


  
}

void shuffle(char pos[]) {
  for (int i = 0; i < char_amount; i++) {
    int r = TrueRandom.random(0,char_amount);
    char temp = pos[i]; pos[i] = pos[r]; pos[r] = temp;
  }
}

void loop() {

  if (!won) {
    if (digitalRead(UP_0_BTN) == 0) {
      int newPos = (positions[0] + 1) % char_amount;
      positions[0] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(DOWN_0_BTN) == 0) {

      int newPos = positions[0] - 1;
      if (newPos < 0 ) {
        newPos = char_amount - 1;
      }
      positions[0] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(UP_1_BTN) == 0) {
      int newPos = (positions[1] + 1) % char_amount;
      positions[1] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(DOWN_1_BTN) == 0) {

      int newPos = positions[1] - 1;
      if (newPos < 0 ) {
        newPos = char_amount - 1;
      }
      positions[1] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(UP_2_BTN) == 0) {
      int newPos = (positions[2] + 1) % char_amount;
      positions[2] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(DOWN_2_BTN) == 0) {

      int newPos = positions[2] - 1;
      if (newPos < 0 ) {
        newPos = char_amount - 1;
      }
      positions[2] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(UP_3_BTN) == 0) {
      int newPos = (positions[3] + 1) % char_amount;
      positions[3] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(DOWN_3_BTN) == 0) {

      int newPos = positions[3] - 1;
      if (newPos < 0 ) {
        newPos = char_amount - 1;
      }
      positions[3] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(UP_4_BTN) == 0) {
      int newPos = (positions[4] + 1) % char_amount;
      positions[4] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(DOWN_4_BTN) == 0) {

      int newPos = positions[4] - 1;
      if (newPos < 0 ) {
        newPos = char_amount - 1;
      }
      positions[4] = newPos;
      printToLcd();
      delay(BTN_PRESS_DELAY);
    } else if (digitalRead(INPUT_ANSWER) == 0) {
      if (GetWord() == password) {
        won = true;
        digitalWrite(VICTORY_LED, HIGH);
      }
      
      delay(BTN_PRESS_DELAY);
    }
  }
}

String GetWord() {
  char toPrint[5] = {posibilities[0][positions[0]], posibilities[1][positions[1]], posibilities[2][positions[2]], posibilities[3][positions[3]], posibilities[4][positions[4]]};
  return toPrint;
}

void printToLcd() {

  lcd.clear();
  char toPrint[16] = {' ', ' ', ' ', ' ', posibilities[0][positions[0]],  ' ', posibilities[1][positions[1]], ' ', posibilities[2][positions[2]], ' ', posibilities[3][positions[3]], ' ', posibilities[4][positions[4]], ' ', ' ', ' '};
  lcd.print(toPrint);

}
