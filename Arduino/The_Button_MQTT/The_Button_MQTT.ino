#include <LinkedList.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <ClientMQTT.h>


#define I2C_ADDR    0x3F
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define STRIP_LED_BLUE 3
#define STRIP_LED_GREEN 5
#define STRIP_LED_RED 6

#define BUTTON_LED_BLUE 9
#define BUTTON_LED_GREEN 10
#define BUTTON_LED_RED 11

#define BUTTON 2
#define VICTORY_LED 8

#define HOLD_TIME 300

#define YELLOW_CODE "Y"
#define WHITE_CODE "W"
#define GREEN_CODE "G"
#define RED_CODE "R"
#define BLUE_CODE "BLU"

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

String button_text = "ABORT";

int red[] = {255, 0, 0};
int orange[] = {255, 128, 0};
int yellow[] = {255, 255, 0};
int green_yellow[] = {128, 255, 0};
int green[] = {0, 255, 0};
int green_blue[] = {0, 255, 128};
int skyblue[] = {0, 255, 255};
int darker_skyblue[] = {0, 128, 255};
int blue[] = {0, 0, 255};
int purple[] = {127, 0, 255};
int pink[] = {255, 0, 255};
int magenta[] = {255, 0, 127};
int grey[] = {128, 128, 128};
int white[] = {255, 255, 255};
int off[] = {0, 0, 0};

int button_color[] = {0, 0, 0};

int strip_color[] = {0, 0, 0};

String button_action_status;

bool won;

const char* ssid = "SaAP";
const char* password = "santiago17";
const char* mqtt_server = "192.168.43.204";
const unsigned int port = 1883;
const char* module = "TheButton";
ClientMQTT client(module);

int * extractColorFromText(String color) {

   if (color == "R") {
      return red;
   } else if (color == "O" ) {
      return orange;
   } else if (color == "Y" ) {
      return yellow;
   } else if (color == "GY" ) {
      return green_yellow;
   } else if (color == "G" ) {
      return green;
   } else if (color == "GBLU" ) {
      return green_blue;
   } else if (color == "SBLU" ) {
      return skyblue;
   } else if (color == "DSBLU" ) {
      return darker_skyblue;
   } else if (color == "BLU" ) {
      return blue;
   } else if (color == "PU" ) {
      return purple;
   } else if (color == "PI" ) {
      return pink;
   } else if (color == "M" ) {
      return magenta;
   } else if (color == "G" ) {
      return grey;
   } else if (color == "W" ) {
      return white;
   } else {
      return off;
  }
}

void f_Start(char* message) {

  int wordsExtracted = 0;
  String s = "";

  for(int i = 0; message[i] != '\0' && wordsExtracted < 3; i++) {

    if (message[i] == ',') {
      //Button color
       if (wordsExtracted == 0) {
         copyColorArray(extractColorFromText(s), button_color);
        //Button text
       } else if (wordsExtracted == 1) {
        button_text = s;
       }
      wordsExtracted++;
      s = "";
    } else {
      s += message[i];
    }
  }

  //Strip Color
  copyColorArray(extractColorFromText(s), strip_color);
  setColor("BUTTON", button_color);
  printToLcd(button_text);
}

void f_End(char* message) {
  won = true;
  digitalWrite(VICTORY_LED, LOW);
  lcd.clear();
  setColor("STRIP", off);
  setColor("BUTTON", off);
}

void f_OnMessage(char* message) {
  if (strcmp(message, "OK") == 0) {
    digitalWrite(VICTORY_LED, HIGH);
    won = true;
  } else if (strcmp(message, "HOLDING") == 0) {
    button_action_status = "HOLD";
    setColor("STRIP", strip_color);
  } else if (strcmp(message, "ERROR") == 0) {
    button_action_status = "";
    setColor("STRIP", off);
  }
}

void setup()
{
  Serial.begin(9600);
  client.connectWIFI(ssid, password);
  client.connectMQTT(mqtt_server, port, f_Start, f_End, f_OnMessage);
  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(VICTORY_LED, OUTPUT);

  button_action_status = "";

  won = false;
}

void loop()
{
  if(!client.isListening()) {
    client.startListeningLoop();
  }
  client.refresh();
  if (!won) {
    if (digitalRead(BUTTON) == LOW && button_action_status == "") {
      delay(HOLD_TIME);
      
      if (digitalRead(BUTTON) == LOW) {
        button_action_status = "HOLD";
        
        Serial.println(button_action_status);
      } else {
        button_action_status = "PRESS";
        Serial.println(button_action_status);
      }
    } else if (digitalRead(BUTTON) == HIGH && button_action_status == "HOLD") {
      button_action_status = "RELEASE";
      
      Serial.println(button_action_status);
    }

    if (button_action_status != "") {
        char* message = client.sendMessageAndWaitForResponse(button_action_status.c_str());
        f_OnMessage(message);
    }
  }
}


void printToLcd(String message) {

  lcd.clear();
  lcd.setCursor(printHereToCenter(message), 0);
  lcd.print(message);


}

int printHereToCenter(String message) {

  int messageLength = message.length();
  int emptySpaces = 16 - messageLength;
  int halfEmpty = emptySpaces/2;

  return halfEmpty;
}

void copyColorArray(int base[], int result[]) {

  for (int i = 0; i < 3; i++) {
    result[i] = base[i];
  }
}

void setColor(String led, int color[])
{
  if (led == "BUTTON") {
    analogWrite(BUTTON_LED_RED, color[0]);
    analogWrite(BUTTON_LED_GREEN,color[1]);
    analogWrite(BUTTON_LED_BLUE,color[2]);  
  } else {
    analogWrite(STRIP_LED_RED, color[0]);
    analogWrite(STRIP_LED_GREEN,color[1]);
    analogWrite(STRIP_LED_BLUE,color[2]);  
  }
}



