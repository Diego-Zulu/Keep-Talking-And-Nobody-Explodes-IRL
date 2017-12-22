#include <LinkedList.h>

#define FIRST_WIRE 2
#define SECOND_WIRE 3
#define THIRD_WIRE 4
#define FOURTH_WIRE 5
#define FIFTH_WIRE 6
#define SIXTH_WIRE 7

#define VICTORY_LED 8
#define TOTAL_WIRES 6

int wiresPos[] = {FIRST_WIRE, SECOND_WIRE, THIRD_WIRE, FOURTH_WIRE, FIFTH_WIRE, SIXTH_WIRE};

int wires_amount;
int correct_wire_count;
int correct_wire;
bool won;
bool winsThisRound;
bool errorThisRound;
LinkedList<int> *wiresActive = new LinkedList<int>();

void setup() {
  Serial.begin(9600);

  pinMode(VICTORY_LED, OUTPUT);
  won = false;
  wires_amount = 3;
  int correct_wire_count = 2;


  for (int i = 0; i < TOTAL_WIRES && wiresActive->size() < wires_amount; i++) {
    pinMode(wiresPos[i], INPUT_PULLUP);
      Serial.println(wiresPos[i]);
    if (digitalRead(wiresPos[i]) == LOW) {
      wiresActive->add(wiresPos[i]);
      if (correct_wire_count == 0) {
        correct_wire = wiresPos[i];
      }
      correct_wire_count--;
    }
  }
   Serial.println(correct_wire);

}

void loop() {

  if (!won) {
    winsThisRound = false;
    errorThisRound = false;

    for (int i = 0; i < wiresActive->size() && !errorThisRound; i++) {
          Serial.println(correct_wire);
      if (digitalRead(wiresActive->get(i)) == HIGH) {
        if (correct_wire == wiresActive->get(i)) {
          winsThisRound = true;
        } else {
          errorThisRound = true;
          wiresActive->remove(i);
          i--;
        } 
      }
    }

    if (winsThisRound) {
      digitalWrite(VICTORY_LED, HIGH);
      won = true;
    } else if (errorThisRound) {
      
    }
  }
}
