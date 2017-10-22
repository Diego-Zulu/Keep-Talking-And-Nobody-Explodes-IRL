#include <TrueRandom.h>
#include <LinkedList.h>
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

#define STRIP_LED_BLUE 2
#define STRIP_LED_GREEN 3
#define STRIP_LED_RED 4

#define BUTTON_LED_BLUE 5
#define BUTTON_LED_GREEN 6
#define BUTTON_LED_RED 7

#define BUTTON 8
#define VICTORY_LED 9

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

String button_text = "ABORT";

int blue[] = {0, 0, 255};
int red[] = {255, 0, 0};
int yellow[] = {255, 255, 0};
int white[] = {255, 255, 255};

int button_color[] = {0, 0, 0};

int strip_color[] = {0, 0, 0};

int randomColor[] = {0, 0, 0};

void setup()
{

  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  Serial.begin(9600);

  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(STRIP_LED_BLUE, OUTPUT);
  pinMode(STRIP_LED_GREEN, OUTPUT);
  pinMode(STRIP_LED_RED, OUTPUT);
  pinMode(BUTTON_LED_BLUE, OUTPUT);
  pinMode(BUTTON_LED_GREEN, OUTPUT);
  pinMode(BUTTON_LED_RED, OUTPUT);
  pinMode(VICTORY_LED, OUTPUT);

  printToLcd(button_text);

  randomColor[0] = TrueRandom.random(0, 256);
  randomColor[1] = TrueRandom.random(0, 256);
  randomColor[2] = TrueRandom.random(0, 256);

  copyColorArray(button_color, white);
  copyColorArray(strip_color, randomColor);

  setColor("BUTTON", red);
  setColor("STRIP", blue);
}

void loop()
{


}


void printToLcd(String message) {

  lcd.clear();
  String centered = wordCenteredForLcd(message);

  Serial.println(centered);
  lcd.print(centered);


}

String wordCenteredForLcd(String message) {

  int messageLength = message.length();
  int emptySpaces = 16 - messageLength;
  int halfEmpty = emptySpaces/2;

  while (emptySpaces > halfEmpty) {
    message = " " + message;
    emptySpaces--;
  }

  while (emptySpaces > 0) {
    message = message + " ";
    emptySpaces--;
  }
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



