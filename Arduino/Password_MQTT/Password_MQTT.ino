
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

#define DOWN_0_BTN 7
#define DOWN_1_BTN 8
#define DOWN_2_BTN 9
#define DOWN_3_BTN 10
#define DOWN_4_BTN 11
#define VICTORY_LED 12
#define INPUT_ANSWER 13

#define BTN_PRESS_DELAY 300
#define char_amount 5

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

String password = "";

char posibilities[char_amount][char_amount];

int positions[char_amount];

bool won = false;

const char* ssid = "SaAP";
const char* password = "santiago17";
const char* mqtt_server = "192.168.43.204";
const unsigned int port = 1883;
const char* module = "Password";
ClientMQTT client(module);

void f_Start(char* message) {
  
  password = String(password + message);
  won = false;
  digitalWrite(VICTORY_LED, LOW);
}

void f_End(char* message) {
  won = true;
}

void f_OnMessage(char* message) {
  if (strcmp(message, "OK") == 0) {
    digitalWrite(VICTORY_LED, HIGH);
    won = true;
  }
}

void setup() {

  Serial.begin(9600);
  client.connectWIFI(ssid, password);
  client.connectMQTT(mqtt_server, port, f_Start, f_End, f_OnMessage);
  


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

  pinMode(DOWN_0_BTN, INPUT_PULLUP);
  pinMode(DOWN_1_BTN, INPUT_PULLUP);
  pinMode(DOWN_2_BTN, INPUT_PULLUP);
  pinMode(DOWN_3_BTN, INPUT_PULLUP);
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
    if (digitalRead(DOWN_0_BTN) == 0) {

      int newPos = positions[0] - 1;
      if (newPos < 0 ) {
        newPos = char_amount - 1;
      }
      positions[0] = newPos;
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
    } else if (digitalRead(DOWN_2_BTN) == 0) {

      int newPos = positions[2] - 1;
      if (newPos < 0 ) {
        newPos = char_amount - 1;
      }
      positions[2] = newPos;
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
