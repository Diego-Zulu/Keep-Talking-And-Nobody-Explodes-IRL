#include <LinkedList.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define SDA_PIN 0
#define SCL_PIN 2

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

#define BUTTON 12
#define VICTORY_LED 13

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

void setup()
{

  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  Serial.begin(9600);

  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(VICTORY_LED, OUTPUT);

  printToLcd(button_text);


  copyColorArray(white, button_color);
  copyColorArray(red, strip_color);

  setColor("BUTTON", button_color);

  button_action_status = "";

  won = false;
}

void loop()
{
  if (!won) {
    if (digitalRead(BUTTON) == LOW && button_action_status == "") {
      delay(HOLD_TIME);
      
      if (digitalRead(BUTTON) == LOW) {
        button_action_status = "HOLD";
        setColor("STRIP", strip_color);
        Serial.println(button_action_status);
      } else {
        button_action_status = "PRESS";
        Serial.println(button_action_status);
      }
    } else if (digitalRead(BUTTON) == HIGH && button_action_status == "HOLD") {
      button_action_status = "RELEASE";
      Serial.println(button_action_status);
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



