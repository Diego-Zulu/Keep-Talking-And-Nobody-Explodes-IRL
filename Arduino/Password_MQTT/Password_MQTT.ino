#include <ClientMQTT.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x3F
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define DOWN_0_BTN D3
#define DOWN_1_BTN D4
#define DOWN_2_BTN D5
#define DOWN_3_BTN D6
#define DOWN_4_BTN D7
#define VICTORY_LED D8
#define INPUT_ANSWER D9

#define BTN_PRESS_DELAY 300
#define char_amount 5

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

String solution = "";

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
  solution = "";
  solution = String(solution + message);
  won = false;
  digitalWrite(VICTORY_LED, LOW);

  for (int i = 0; i < char_amount; i++) {
    posibilities[i][0] = solution[i];
    for (int j = 1; j < char_amount; j++) {
      randomSeed(millis());
      bool repeated = true;
      while (repeated) {
        repeated = false;
        char cch = 'A' + random(0,26);
        for (int f = 0; f <= j; f++) {
          if (posibilities[i][f] == cch) {
            repeated = true;
            break;
          }
        }
        posibilities[i][j] = cch;
      }
    }
    shuffle(posibilities[i]);
  }

    printToLcd();
}

void f_End(char* message) {
  won = true;
  lcd.clear();
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

  for (int i = 0; i < char_amount; i++) {
    positions[i] = 0;
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


  
}

void shuffle(char pos[]) {
  randomSeed(millis());
  for (int i = 0; i < char_amount; i++) {
    
    int r = random(0,char_amount);
    char temp = pos[i]; pos[i] = pos[r]; pos[r] = temp;
  }
}

void loop() {
  if(!client.isListening()) {
    client.startListeningLoop();
  }
  client.refresh();
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
     string s = GetWord();
     Serial.println("SEND");
     Serial.println(s.c_str());
     client.sendMessage(s.c_str());
     client.refresh();
     delay(BTN_PRESS_DELAY);
    }
  }
}

string GetWord() {
  char toPrint[6] = {posibilities[0][positions[0]], posibilities[1][positions[1]], posibilities[2][positions[2]], posibilities[3][positions[3]], posibilities[4][positions[4]], '\0'};
  return toPrint;
}

void printToLcd() {

  lcd.clear();
  char toPrint[16] = {' ', ' ', posibilities[0][positions[0]],  ' ', ' ', posibilities[1][positions[1]], ' ',' ', posibilities[2][positions[2]], ' ', ' ',posibilities[3][positions[3]], ' ', ' ',posibilities[4][positions[4]],  ' '};
  lcd.print(toPrint);
}
