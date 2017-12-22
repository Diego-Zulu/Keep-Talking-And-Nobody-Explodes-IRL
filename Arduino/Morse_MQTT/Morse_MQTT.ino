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

#define BACK_BTN D3
#define FORW_BTN D4
#define ACCEPT_BTN D5
#define CODE_LED D7
#define BIG_LED D6
#define FREQUENCIES 16

#define dotLen 400     // length of the morse code 'dot'
#define dashLen 900    // length of the morse code 'dash'
#define elemPause 400  // length of the pause between elements of a character
#define Spaces 900    // length of the spaces between characters
#define wordPause 2500  // length of the pause between words
#define lcd_print_delay 100
#define time_between_delays 50


LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

String words[FREQUENCIES] = {"3.505 MHz", "3.515 MHz", "3.522 MHz", "3.532 MHz", "3.535 MHz",
                             "3.542 MHz", "3.545 MHz", "3.552 MHz", "3.555 MHz", "3.565 MHz",
                             "3.572 MHz", "3.575 MHz", "3.582 MHz", "3.592 MHz", "3.595 MHz", "3.600 MHz"
                            };

String tuner[FREQUENCIES] = {"|..!..!..!..!..!", "!|.!..!..!..!..!","!.|!..!..!..!..!","!..|..!..!..!..!",
                            "!..!|.!..!..!..!","!..!.|!..!..!..!","!..!..|..!..!..!","!..!..!|.!..!..!",
                             "!..!..!.|!..!..!","!..!..!..|..!..!","!..!..!..!|.!..!","!..!..!..!.|!..!",
                             "!..!..!..!..|..!","!..!..!..!..!|.!","!..!..!..!..!.|!","!..!..!..!..!..|",};


int pos = 0;

String code = "... .... . .-.. .-..";

String correct = "3.552 MHz";

bool won = false;
bool started = false;
bool waitingForServer = false;

LinkedList<String> *buttonsQueue = new LinkedList<String>();
LinkedList<String> *morseQueue = new LinkedList<String>();

const char* ssid = "SaAP";
const char* password = "santiago17";
const char* mqtt_server = "192.168.43.204";
const unsigned int port = 1883;
const char* module = "MorseCode";
ClientMQTT client(module);

void f_Start(char* message) {
  Serial.println("START");
  won = false;
  digitalWrite(BIG_LED, LOW);
  buttonsQueue->add("CHECK");
  morseQueue->add("CHAR");
  morseQueue->add(String(0));
  printFreqToLcd(0);
  started = true;
  waitingForServer = false;
  pos = 0;
}

void f_End(char* message) {
  Serial.println("ONEND");
  won = true;
  printFreqToLcd(-1);
  started = false;
  waitingForServer = false;
  digitalWrite(CODE_LED, LOW);
  digitalWrite(BIG_LED, LOW);
}

void f_OnMessage(char* message) {
  Serial.println("ONMESSAGE");
  Serial.println(message);
  if (strcmp(message, "OK") == 0) {
    digitalWrite(BIG_LED, HIGH);
     digitalWrite(CODE_LED, LOW);
    won = true;
    started = false;
      buttonsQueue->clear();
      morseQueue->clear();
  }
  waitingForServer = false;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("SETUP");
  client.connectWIFI(ssid, password);
  client.connectMQTT(mqtt_server, port, f_Start, f_End, f_OnMessage);
  Serial.println("CONNECTED");
  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  pinMode(BACK_BTN, INPUT_PULLUP);
  pinMode(FORW_BTN, INPUT_PULLUP);
  pinMode(ACCEPT_BTN, INPUT_PULLUP);
  pinMode(CODE_LED, OUTPUT);
  pinMode(BIG_LED, OUTPUT);
  digitalWrite(BIG_LED, LOW);

  //printFreqToLcd(0);
}

void loop()
{
  if(!client.isListening()) {
    client.startListeningLoop();
  }
  client.refresh();
  if (started) {
      ConsumeMorseQueue();
  ConsumeCheckButtonsQueue();
  }

}

void ConsumeMorseQueue() {
  if (morseQueue->size() > 0) {
    String action = morseQueue->shift();

    if (action == "LIGHTSOFF") {
      String lightsOfTime = morseQueue->shift();
      int t = lightsOfTime.toInt();
      LightsOff(t);

    } else if (action == "CHAR") {
      String charPos = morseQueue->shift();
      int pos = charPos.toInt();
      char tmpChar = code[pos];
      if (tmpChar == '\0') {
        pos = -1;
        LightsOff(wordPause);
      } else {
        shineChar(tmpChar);
      }
      morseQueue->add("CHAR");
      morseQueue->add(String(pos + 1));

    } else if (action == "DELAY") {
      doTinyDelayAndReque(morseQueue);
    }


  }
}

void ConsumeCheckButtonsQueue() {

  if (buttonsQueue->size() > 0) {
    String action = buttonsQueue->shift();

    if (action == "CHECK") {
      buttonsQueue->add("CHECK");
      checkButtons();
    } else if (action == "DELAY") {

      doTinyDelayAndReque(buttonsQueue);

    }
  }
}

void doTinyDelayAndReque(LinkedList<String>* list) {
  String delayTime = list->shift();
  
  int t = delayTime.toInt();

  delay(time_between_delays);
  t -= time_between_delays;

  if (t > 0) {
    addDelay(list, t);
  }
}

void shineChar(char c) {

  if (c == '-') {
    MorseDash();
  } else if (c == '.') {
    MorseDot();
  } else {
    LightsOff(Spaces);
  }
}

// DOT
void MorseDot()
{
  digitalWrite(CODE_LED, HIGH);    // turn the LED on
  addDelay(morseQueue, dotLen);              // hold in this position
  addLightsOffDelay(elemPause);
  //LightsOff(elemPause);
}

// DASH
void MorseDash()
{
  digitalWrite(CODE_LED, HIGH);    // turn the LED on
  addDelay(morseQueue, dashLen);               // hold in this position
  addLightsOffDelay(elemPause);
}

void addLightsOffDelay(int t) {
  morseQueue->add("LIGHTSOFF");
  morseQueue->add(String(t)); 
}

// Turn Off
void LightsOff(int delayTime)
{
  digitalWrite(CODE_LED, LOW);    // turn the LED off
  addDelay(morseQueue, delayTime);             // hold in this position
}

static int checkButtons() {
  if (!won) {
    if (digitalRead(BACK_BTN) == 0) {

      pos--;
      if (pos < 0) {
        pos = 0;
      } else {
        printFreqToLcd(pos);
        addDelay(buttonsQueue, lcd_print_delay);
      }


    } else if (digitalRead(FORW_BTN) == 0) {

      pos++;
      if (pos >= FREQUENCIES) {
        pos = FREQUENCIES - 1;
      } else {
        printFreqToLcd(pos);
        addDelay(buttonsQueue, lcd_print_delay);
      }
    } else if (digitalRead(ACCEPT_BTN) == 0 && !waitingForServer) {
        waitingForServer = true;
        Serial.println("SEND");
        
        client.sendMessage(words[pos].c_str());
        delay(150);
        client.refresh();
        
    }
  }
}

void addDelay(LinkedList<String>* list, int t) {
  list->unshift(String(t));
  list->unshift("DELAY");
}


void printFreqToLcd(int pos) {

    lcd.clear();
  if (pos != -1) {
    lcd.setCursor(0,0);
    
    lcd.print(tuner[pos]);
    lcd.setCursor(0,1);
    lcd.print("   " + words[pos] + "   ");
  }
}



