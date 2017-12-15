#include <ClientMQTT.h>

#define CHOICE_OFF      0 //Used to control LEDs
#define CHOICE_NONE     0 //Used to check buttons
#define CHOICE_RED  (1 << 0)
#define CHOICE_GREEN    (1 << 1)
#define CHOICE_BLUE (1 << 2)
#define CHOICE_YELLOW   (1 << 3)

#define LED_RED     13
#define LED_GREEN   5
#define LED_BLUE    14
#define LED_YELLOW  0
#define VICTORY_LED  3

#define BUTTON_RED    15
#define BUTTON_GREEN  4
#define BUTTON_BLUE   12
#define BUTTON_YELLOW 2

#define ROUNDS_TO_WIN      32 
#define ENTRY_TIME_LIMIT   3000 

#define MODE_MEMORY  0
#define MODE_BATTLE  1
#define MODE_BEEGEES 2

string gameBoard[32]; 
int gameBoardLength = 32;
string userInputs[32];
byte gameRound = 0; 
byte currentMove = 0;
bool won = false;
bool waitingForResponse = false;

const char* ssid = "ASUS";
const char* password = "M#V#23622607";
const char* mqtt_server = "192.168.2.150";
const unsigned int port = 1883;
const char* module = "SimonSays";
ClientMQTT client(module);

void sendInputsToServer() {

  waitingForResponse = true;
  string s = "";
  for (int i=0; i<currentMove; i++) {
    s += userInputs[i] + ",";
  }
  s += userInputs[currentMove];
  Serial.println("SEND");
  Serial.println(s.c_str());
  client.sendMessage(s.c_str());
  client.refresh();
}

int countCommasInMessage(char* message) {
  int count = 0;

  for(int i = 0; message[i] != '\0'; i++) {

    if (message[i] == ',') {
      count++;
    }

  }
  
  return ++count;
}

void fillGameBoardWithColors(char* message) {
  int wordsExtracted = 0;
  string s = "";

  for(int i = 0; message[i] != '\0' && wordsExtracted <= gameBoardLength; i++) {

    if (message[i] == ',') {
      gameBoard[wordsExtracted] = s;
      wordsExtracted++;
      s = "";
    } else {
      s.push_back(message[i]);
    }

  }
}

void f_Start(char* message) {
  
 Serial.begin(9600);
   Serial.println("FSTART");
 Serial.println(message);
  int gameBoardLength = countCommasInMessage(message);
  fillGameBoardWithColors(message);
  
  won = false;
  digitalWrite(VICTORY_LED, LOW);
}

void f_End(char* message) {
  won = true;
  Serial.println("FEND");
}

void f_OnMessage(char* message) {
  Serial.println("ONMESSAGE");
  if (strcmp(message, "OK") == 0) {
    gameRound++;
    if (gameRound >= gameBoardLength) {
      digitalWrite(VICTORY_LED, HIGH);
      won = true;
    }

  } else if (strcmp(message, "ERROR") == 0) {
   //clean userInputs
   gameRound = 0;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("SETUP");
  client.connectWIFI(ssid, password);
  client.connectMQTT(mqtt_server, port, f_Start, f_End, f_OnMessage);
  Serial.println("CONECTED");

  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(VICTORY_LED, OUTPUT);
  digitalWrite(VICTORY_LED, LOW);


}

void loop()
{
  if(!client.isListening()) {
    client.startListeningLoop();
  }

    if (won) {
      play_winner(); 
      } else {
        play_memory();
      }
}

boolean play_memory(void)
{

  gameRound = 0;

  while (!won) 
  {
    Serial.println("LOOP");
    client.refresh();
    playMoves();

    for (currentMove = 0 ; currentMove <= gameRound ; currentMove++)
    {
      byte choice = wait_for_button(); // See what button the user presses

      if (choice == 0) {
        currentMove--; 
        playMoves();
      } else {
         if(choice == CHOICE_RED) userInputs[currentMove] = "R";
          else if(choice == CHOICE_GREEN) userInputs[currentMove] = "G";
          else if(choice == CHOICE_BLUE) userInputs[currentMove] = "BLU";
          else if(choice == CHOICE_YELLOW) userInputs[currentMove] = "Y";

          
          sendInputsToServer();
          while (waitingForResponse) {
            client.refresh();
          }
          
      }
    }

    delay(1000);
  }

  return true;
}

void setLEDs(byte leds)
{
  if ((leds & CHOICE_RED) != 0)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);

  if ((leds & CHOICE_GREEN) != 0)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);

  if ((leds & CHOICE_BLUE) != 0)
    digitalWrite(LED_BLUE, HIGH);
  else
    digitalWrite(LED_BLUE, LOW);

  if ((leds & CHOICE_YELLOW) != 0)
    digitalWrite(LED_YELLOW, HIGH);
  else
    digitalWrite(LED_YELLOW, LOW);
}

byte transformMoveIntoByte(int i) {

  if (gameBoard[i] == "R") {
    return CHOICE_RED;
  } else if (gameBoard[i] == "BLU") {
    return CHOICE_BLUE;
  } else if (gameBoard[i] == "G") {
    return CHOICE_GREEN;
  } else if (gameBoard[i] == "Y") {
    return CHOICE_YELLOW;
  }

  return 0;
}

void playMoves(void)
{

  for (int currentMove = 0; currentMove <= gameRound ; currentMove++) 
  {
    byte l = transformMoveIntoByte(currentMove);
      setLEDs(l); //Turn on a given LED

  delay(150);

  setLEDs(CHOICE_OFF);

   delay(150);
  }
}


byte wait_for_button(void)
{
  long startTime = millis(); // Remember the time we started the this loop

  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Loop until too much time has passed
  {
    byte button = checkButton();

    if (button != CHOICE_NONE)
    { 
      while(checkButton() != CHOICE_NONE) ; 

      delay(10); 

      return button;
    }

  }

  return CHOICE_NONE;
}

byte checkButton(void)
{
  if (digitalRead(BUTTON_RED) == 0) return(CHOICE_RED); 
  else if (digitalRead(BUTTON_GREEN) == 0) return(CHOICE_GREEN); 
  else if (digitalRead(BUTTON_BLUE) == 0) return(CHOICE_BLUE); 
  else if (digitalRead(BUTTON_YELLOW) == 0) return(CHOICE_YELLOW);

  return(CHOICE_NONE); // If no button is pressed, return none
}

void play_winner(void)
{
  digitalWrite(VICTORY_LED, HIGH);
  won = true;
}
