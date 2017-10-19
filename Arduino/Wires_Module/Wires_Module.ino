

int wires_amount;
bool won;
bool winsThisRound;

void setup() {

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);

  won = false;
  wires_amount = 3;
}

void loop() {

  if (!won) {
    winsThisRound = false;
  
    if (digitalRead(2) == HIGH) {
      winsThisRound = true;
    }
    if (digitalRead(3) == HIGH) {
       winsThisRound = true;
    }
    if (digitalRead(4) == HIGH) {
       winsThisRound = true;
    }
    if (digitalRead(5) == HIGH && wires_amount > 3) {
       winsThisRound = true;
    }
    if (digitalRead(6) == HIGH && wires_amount > 4) {
      winsThisRound = true;
    }
    if (digitalRead(7) == HIGH && wires_amount > 5) {
       winsThisRound = true;
    }

    if (winsThisRound  ) {
      digitalWrite(8, HIGH);
      won = true;
    }
  }
  

}
