#include <LinkedList.h>

#define FIRST_WIRE 5
#define SECOND_WIRE 4
#define THIRD_WIRE 0
#define FOURTH_WIRE 2
#define FIFTH_WIRE 14
#define SIXTH_WIRE 12

#define LAST_WIRE FIRST_WIRE+5
#define VICTORY_LED 13
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
  int correct_wire_count = 2;
  int correct_wire = FIRST_WIRE;

  for (int i = 0; i <= TOTAL_WIRES && wiresActive->size() < wires_amount; i++) {
    pinMode(wiresPos[i], INPUT_PULLUP);
    if (digitalRead(wiresPos[i]) == LOW) {
      wiresActive->add(wiresPos[i]);
      if (correct_wire_count == 0) {
        correct_wire = wiresPos[i];
      }
      correct_wire_count--;
    }
  }

}

void loop() {
  if (!won) {
    winsThisRound = false;
    errorThisRound = false;

    for (int i = 0; i < wiresActive->size() && !errorThisRound; i++) {
      Serial.println(wiresActive->get(i));
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
