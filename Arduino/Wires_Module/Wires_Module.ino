#include <LinkedList.h>

#define FIRST_WIRE 2
#define LAST_WIRE FIRST_WIRE+5
#define VICTORY_LED 8

int wires_amount;
int correct_wire;
bool won;
bool winsThisRound;
LinkedList<String> *wiresActive = new LinkedList<String>();

void setup() {

  /*pinMode(FIRST_WIRE, INPUT_PULLUP);
  pinMode(FIRST_WIRE+1, INPUT_PULLUP);
  pinMode(FIRST_WIRE+2, INPUT_PULLUP);
  pinMode(FIRST_WIRE+3, INPUT_PULLUP);
  pinMode(FIRST_WIRE+4, INPUT_PULLUP);
  pinMode(LAST_WIRE, INPUT_PULLUP);*/
  
  pinMode(VICTORY_LED, OUTPUT);
  digitalWrite(VICTORY_LED, HIGH);

  /*won = false;
  wires_amount = 3;
  correct_wire = 2;

  for (int i=FIRST_WIRE; i<=LAST_WIRE && wiresActive->size() < wires_amount; i++) {

    if (digitalRead(i) == LOW) {
      wiresActive->add(i);
    }
  }*/

}

void loop() {

 /* if (!won) {
    winsThisRound = false;
    errorThisRound = false;

    for (int i=0; i<wiresActive->size() && !errorThisRound; i++) {
      if (digitalRead(i) == HIGH && correct_wire == (i+1)) {
        winsThisRound = true;
      } else {
        errorThisRound = true;
      }
    }

    if (winsThisRound) {
      digitalWrite(VICTORY_LED, HIGH);
      won = true;
    } else if (errorThisRound) {
      
    }
  }*/
  

}
