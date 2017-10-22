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

#define BACK_BTN 2
#define FORW_BTN 3
#define ACCEPT_BTN 6
#define CODE_LED 4
#define BIG_LED 5
#define FREQUENCIES 16

#define dotLen 400     // length of the morse code 'dot'
#define dashLen 800    // length of the morse code 'dash'
#define elemPause 400  // length of the pause between elements of a character
#define Spaces 800    // length of the spaces between characters
#define wordPause 2400  // length of the pause between words


LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

String words[FREQUENCIES] = {"3.505 MHz", "3.515 MHz", "3.522 MHz", "3.532 MHz", "3.535 MHz",
                             "3.542 MHz", "3.545 MHz", "3.552 MHz", "3.555 MHz", "3.565 MHz",
                             "3.572 MHz", "3.575 MHz", "3.582 MHz", "3.592 MHz", "3.595 MHz", "3.600 MHz"
                            };


int pos = 0;

String code = "... .... . .-.. .-..";

String correct = "3.552 MHz";

bool won = false;


void shineChar(char c) {

  if (c == '-') {
    MorseDash();
    LightsOff(elemPause);
  } else if (c == '.') {
    MorseDot();
    LightsOff(elemPause);
  } else {
    LightsOff(Spaces);
  }
}

// DOT
void MorseDot()
{
  digitalWrite(CODE_LED, HIGH);    // turn the LED on
  delay(dotLen);              // hold in this position
}

// DASH
void MorseDash()
{
  digitalWrite(CODE_LED, HIGH);    // turn the LED on
  delay(dashLen);               // hold in this position
}

// Turn Off
void LightsOff(int delayTime)
{
  digitalWrite(CODE_LED, LOW);    // turn the LED off
  delay(delayTime);             // hold in this position
}

void setup()
{
  Serial.begin(9600);
  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  pinMode(BACK_BTN, INPUT_PULLUP);
  pinMode(FORW_BTN, INPUT_PULLUP);
  pinMode(ACCEPT_BTN, INPUT_PULLUP);
  pinMode(CODE_LED, OUTPUT);
  pinMode(BIG_LED, OUTPUT);

  printToLcd(words[0]);

}

static int DoTheMorse() {
  if (!won) {

    for (int i = 0; !won && code[i] != "\0"; i++)
    {

      char tmpChar = code[i];
      shineChar(tmpChar);
    }

    LightsOff(wordPause);
  }
}

static int checkButtons() {
  if (!won) {
    if (digitalRead(BACK_BTN) == 0) {

      pos--;
      if (pos < 0) {
        pos = 0;
      } else {
        printToLcd(words[pos]);
        delay(300);
      }


    } else if (digitalRead(FORW_BTN) == 0) {

      pos++;
      if (pos >= FREQUENCIES) {
        pos = FREQUENCIES - 1;
      } else {
        printToLcd(words[pos]);
        delay(300);
      }


      //WIN
    } else if (digitalRead(ACCEPT_BTN) == 0) {
      digitalWrite(BIG_LED, HIGH);
      won = true;
    }
  }
}

void loop()
{

  ConsumeList();
  checkButtons();
}



void printToLcd(String message) {
  Serial.println(message);
  lcd.clear();
  lcd.print("   " + message + "   ");
}



