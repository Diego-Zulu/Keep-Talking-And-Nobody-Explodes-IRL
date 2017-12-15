#include <LinkedList.h>
#include <ClientMQTT.h>

#define FIRST_WIRE 5
#define SECOND_WIRE 4
#define THIRD_WIRE 0
#define FOURTH_WIRE 2
#define FIFTH_WIRE 14
#define SIXTH_WIRE 12

#define VICTORY_LED 13
#define TOTAL_WIRES 6

int wiresPos[] = { FIRST_WIRE, SECOND_WIRE, THIRD_WIRE, FOURTH_WIRE, FIFTH_WIRE, SIXTH_WIRE };

int wires_amount;
bool won;
LinkedList<int> *wiresActive = new LinkedList<int>();
LinkedList<bool> *wiresUnused = new LinkedList<bool>();

const char* ssid = "SaAP";
const char* password = "santiago17";
const char* mqtt_server = "192.168.43.204";
const unsigned int port = 1883;
const char* module = "Wires";
ClientMQTT client(module);

void f_Start(char* message) {
  wires_amount = atoi(message);
  won = false;
  wiresActive->clear();
  wiresUnused->clear();
  for (int i = 0; i < TOTAL_WIRES && wiresActive->size() < wires_amount; i++) {
    if (digitalRead(wiresPos[i]) == LOW) {
      wiresActive->add(wiresPos[i]);
      wiresUnused->add(true);
    }
  }
  digitalWrite(VICTORY_LED, LOW);
}

void f_End(char* message) {
  won = true;
}

void f_OnMessage(char* message) {
  if (strcmp(message, "OK") == 0) {
    digitalWrite(VICTORY_LED, HIGH);
    won = true;
  }
}

void setup() {
  Serial.begin(9600);
  client.connectWIFI(ssid, password);
  client.connectMQTT(mqtt_server, port, f_Start, f_End, f_OnMessage);
  
  pinMode(VICTORY_LED, OUTPUT);
  for (int i = 0; i < TOTAL_WIRES; i++) {
    pinMode(wiresPos[i], INPUT_PULLUP);
  }
  //won = true;
}

void loop() {
  if(!client.isListening()) {
    client.startListeningLoop();
  }
  client.refresh();
  if (!won) {
    for (int i = 0; i < wiresActive->size(); i++) {
      if (!wiresUnused->get(i)) {
        continue;
      }
      if (digitalRead(wiresActive->get(i)) == HIGH) {
        string s;
        s.push_back((char)('0' + i));
        client.sendMessage(s.c_str());
        client.refresh();
        wiresUnused->set(i, false);
      }
    }
  }
}
