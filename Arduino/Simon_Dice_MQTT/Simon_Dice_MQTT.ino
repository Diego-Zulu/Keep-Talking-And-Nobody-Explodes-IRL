#include <ClientMQTT.h>

#define CHOICE_OFF      0 //Used to control LEDs
#define CHOICE_NONE     0 //Used to check buttons
#define CHOICE_RED  (1 << 0)
#define CHOICE_GREEN    (1 << 1)
#define CHOICE_BLUE (1 << 2)
#define CHOICE_YELLOW   (1 << 3)

#define LED_RED     9
#define LED_GREEN   7
#define LED_BLUE    5
#define LED_YELLOW  3
#define VICTORY_LED  2

// Button pin definitions
#define BUTTON_RED    10
#define BUTTON_GREEN  8
#define BUTTON_BLUE   6
#define BUTTON_YELLOW 4

#define ENTRY_TIME_LIMIT   3000 //Amount of time to press a button before game times out. 3000ms = 3 sec

byte gameBoard[32]; //Contains the combination of buttons as we advance
int gameBoardLength = 32;
string userInputs[32];
byte gameRound = 0; //Counts the number of succesful rounds the player has made it through
bool won = false;

const char* ssid = "ASUS";
const char* password = "M#V#23622607";
const char* mqtt_server = "192.168.2.150";
const unsigned int port = 1883;
const char* module = "SimonSays";
ClientMQTT client(module);

bool sendInputsToServerAndCheckIfOkay(int currentMove) {

  string s = "";
  for (int i=0; i<currentMove; i++) {
    s += userInputs[i] + ",";
  }
  s += userInputs[currentMove];
  Serial.println("SEND");
  Serial.println(s.c_str());
  char* message = client.sendMessageAndWaitForResponse(s.c_str());
  return strcmp(message, "OK") == 0;
}

int countCommasInMessage(char* message) {
  int count = 0;

  for(int i = 0; message[i] != '\0'; i++) {

    if (message[i] == ',') {
      count++;
    }

  }
  
  return count;
}

void fillGameBoardWithColors(char* message) {
  int wordsExtracted = 0;
  string s = "";

  for(int i = 0; message[i] != '\0' && wordsExtracted < gameBoardLength; i++) {

    if (message[i] == ',') {
       if(s == "R") gameBoard[wordsExtracted] = CHOICE_RED;
          else if(s == "G") gameBoard[wordsExtracted] = CHOICE_GREEN;
          else if(s == "BLU") gameBoard[wordsExtracted] = CHOICE_BLUE;
          else if(s == "Y") gameBoard[wordsExtracted] = CHOICE_YELLOW;
      wordsExtracted++;
      s = "";
    } else {
      s.push_back(message[i]);
    }

  }

  if(s == "R") gameBoard[wordsExtracted] = CHOICE_RED;
          else if(s == "G") gameBoard[wordsExtracted] = CHOICE_GREEN;
          else if(s == "BLU") gameBoard[wordsExtracted] = CHOICE_BLUE;
          else if(s == "Y") gameBoard[wordsExtracted] = CHOICE_YELLOW;
}

void f_Start(char* message) {
  
 Serial.begin(9600);
   Serial.println("FSTART");
 Serial.println(message);
  int gameBoardLength = countCommasInMessage(message) + 1;
  fillGameBoardWithColors(message);
  
  won = false;
  digitalWrite(VICTORY_LED, LOW);
}

void f_End(char* message) {
  won = true;
  digitalWrite(VICTORY_LED, LOW);
  Serial.println("FEND");
}

void f_OnMessage(char* message) {
  Serial.println("ONMESSAGE");
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
  client.refresh();

  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE | CHOICE_YELLOW); 
  delay(1000);
  setLEDs(CHOICE_OFF); 
  delay(250);

    if (!won && play_memory() == true) 
      play_winner(); // Player won, play winner tones
}

boolean play_memory(void)
{

  gameRound = 0; // Reset the game to the beginning

  while (gameRound < gameBoardLength) 
  {
    gameRound++;

    playMoves(); // Play back the current game board

    // Then require the player to repeat the sequence.
    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)
    {
      client.refresh();

      byte choice = wait_for_button(); // See what button the user presses

      if (choice == 0) {
        currentMove--; 
        playMoves();
      } else {
         //if (choice != correctInputs[currentMove]) return false; // If the choice is incorect, player loses
        
         if(choice == CHOICE_RED) userInputs[currentMove] = "R";
          else if(choice == CHOICE_GREEN) userInputs[currentMove] = "G";
          else if(choice == CHOICE_BLUE) userInputs[currentMove] = "BLU";
          else if(choice == CHOICE_YELLOW) userInputs[currentMove] = "Y";

          if (!sendInputsToServerAndCheckIfOkay(currentMove)) {
            return false;
          }
      }
    }

    delay(1000); // Player was correct, delay before playing moves
  }

  return true; // Player made it through all the rounds to win!
}

// Plays the current contents of the game moves
void playMoves(void)
{
  for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) 
  {
    toner(gameBoard[currentMove], 150);

    // Wait some amount of time between button playback
    // Shorten this to make game harder
    delay(150); // 150 works well. 75 gets fast.
  }
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//The following functions control the hardware

// Lights a given LEDs
// Pass in a byte that is made up from CHOICE_RED, CHOICE_YELLOW, etc
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

// Wait for a button to be pressed. 
// Returns one of LED colors (LED_RED, etc.) if successful, 0 if timed out
byte wait_for_button(void)
{
  long startTime = millis(); // Remember the time we started the this loop

  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Loop until too much time has passed
  {
    byte button = checkButton();

    if (button != CHOICE_NONE)
    { 
      toner(button, 150); // Play the button the user just pressed

      while(checkButton() != CHOICE_NONE) ;  // Now let's wait for user to release button

      delay(10); // This helps with debouncing and accidental double taps

      return button;
    }

  }

  return CHOICE_NONE; // If we get here, we've timed out!
}

// Returns a '1' bit in the position corresponding to CHOICE_RED, CHOICE_GREEN, etc.
byte checkButton(void)
{
  if (digitalRead(BUTTON_RED) == 0) return(CHOICE_RED); 
  else if (digitalRead(BUTTON_GREEN) == 0) return(CHOICE_GREEN); 
  else if (digitalRead(BUTTON_BLUE) == 0) return(CHOICE_BLUE); 
  else if (digitalRead(BUTTON_YELLOW) == 0) return(CHOICE_YELLOW);

  return(CHOICE_NONE); // If no button is pressed, return none
}

// Light an LED and play tone
// Red, upper left:     440Hz - 2.272ms - 1.136ms pulse
// Green, upper right:  880Hz - 1.136ms - 0.568ms pulse
// Blue, lower left:    587.33Hz - 1.702ms - 0.851ms pulse
// Yellow, lower right: 784Hz - 1.276ms - 0.638ms pulse
void toner(byte which, int buzz_length_ms)
{
  setLEDs(which); //Turn on a given LED

  //Play the sound associated with the given LED
  switch(which) 
  {
  case CHOICE_RED:
    buzz_sound(buzz_length_ms, 1136); 
    break;
  case CHOICE_GREEN:
    buzz_sound(buzz_length_ms, 568); 
    break;
  case CHOICE_BLUE:
    buzz_sound(buzz_length_ms, 851); 
    break;
  case CHOICE_YELLOW:
    buzz_sound(buzz_length_ms, 638); 
    break;
  }

  setLEDs(CHOICE_OFF); // Turn off all LEDs
}

// Toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms.
void buzz_sound(int buzz_length_ms, int buzz_delay_us)
{
  // Convert total play time from milliseconds to microseconds
  long buzz_length_us = buzz_length_ms * (long)1000;

  // Loop until the remaining play time is less than a single buzz_delay_us
  while (buzz_length_us > (buzz_delay_us * 2))
  {
    buzz_length_us -= buzz_delay_us * 2; //Decrease the remaining play time

    //digitalWrite(BUZZER1, LOW);
    //digitalWrite(BUZZER2, HIGH);
    delayMicroseconds(buzz_delay_us);

    //digitalWrite(BUZZER1, HIGH);
    //digitalWrite(BUZZER2, LOW);
    delayMicroseconds(buzz_delay_us);
  }
}

// Play the winner sound and lights
void play_winner(void)
{
  digitalWrite(VICTORY_LED, HIGH);
  won = true;
}

// Play the winner sound
// This is just a unique (annoying) sound we came up with, there is no magic to it
void winner_sound(void)
{
  // Toggle the buzzer at various speeds
  for (byte x = 250 ; x > 70 ; x--)
  {
    for (byte y = 0 ; y < 3 ; y++)
    {
      //digitalWrite(BUZZER2, HIGH);
      //digitalWrite(BUZZER1, LOW);
      delayMicroseconds(x);

      //digitalWrite(BUZZER2, LOW);
      //digitalWrite(BUZZER1, HIGH);
      delayMicroseconds(x);
    }
  }
}

// Play the loser sound/lights
void play_loser(void)
{
 delay(1000);
}

int noteDuration = 115; // This essentially sets the tempo, 115 is just about right for a disco groove :)
int LEDnumber = 0; // Keeps track of which LED we are on during the beegees loop

// Do nothing but play bad beegees music
// This function is activated when user holds bottom right button during power up
void play_beegees()
{
  //Turn on the bottom right (yellow) LED
  setLEDs(CHOICE_YELLOW);
  toner(CHOICE_YELLOW, 150);

  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE); // Turn on the other LEDs until you release button

  while(checkButton() != CHOICE_NONE) ; // Wait for user to stop pressing button

  setLEDs(CHOICE_NONE); // Turn off LEDs

  delay(1000); // Wait a second before playing song

  //digitalWrite(BUZZER1, LOW); // setup the "BUZZER1" side of the buzzer to stay low, while we play the tone on the other pin.

  while(checkButton() == CHOICE_NONE) //Play song until you press a button
  {
    // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 32; thisNote++) {
      changeLED();
      //tone(BUZZER2, melody[thisNote],noteDuration);
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      //noTone(BUZZER2);
    }
  }
}

// Each time this function is called the board moves to the next LED
void changeLED(void)
{
  setLEDs(1 << LEDnumber); // Change the LED

  LEDnumber++; // Goto the next LED
  if(LEDnumber > 3) LEDnumber = 0; // Wrap the counter if needed
}
