#include <ClientMQTT.h>

const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASS";
const char* mqtt_server = "IP_SERVER_MQTT";
const unsigned int port = 1883;
const char* module = "Wires";
ClientMQTT client(module);

void f_Start(char* message) {
  Serial.println("HELLI: START");
}

void f_End(char* message) {
  Serial.println("HELLI: END");
  started = false;
}

void f_OnMessage(char* message) {
  Serial.println("HELLI: ON MESSAGE");
}

void setup() {
  Serial.begin(9600);
  client.connectWIFI(ssid, password);
  client.connectMQTT(mqtt_server, port, f_Start, f_End, f_OnMessage);
}

void loop() {
  if(!client.isListening()) {
    client.startListeningLoop();
  }
  client.refresh();
  delay(1000);
  /*
  Serial.println("WITH WAITING");
  client.sendMessage("0");
  client.Loop();
  */
  Serial.println("WITHOUT WAITING");
  char* response = client.sendMessageAndWaitForResponse("1");
  Serial.print("RESPONSE: ");
  Serial.println(response);
  delete response;
  /**/
}
