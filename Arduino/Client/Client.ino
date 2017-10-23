#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string>
using namespace std;

#ifndef CLIENT_MQTT_IOT

#define DEBUG true

#define IOT_CALLBACK_SIGNATURE_PARAMETERS (char*)
#define MQTT_IOT_CALLBACK_SIGNATURE_STATIC_START void (*ClientMQTT::Start)IOT_CALLBACK_SIGNATURE_PARAMETERS
#define MQTT_IOT_CALLBACK_SIGNATURE_START void (*Start)IOT_CALLBACK_SIGNATURE_PARAMETERS
#define MQTT_IOT_CALLBACK_SIGNATURE_STATIC_END void (*ClientMQTT::End)IOT_CALLBACK_SIGNATURE_PARAMETERS
#define MQTT_IOT_CALLBACK_SIGNATURE_END void (*End)IOT_CALLBACK_SIGNATURE_PARAMETERS
#define MQTT_IOT_CALLBACK_SIGNATURE_STATIC_MESSAGE void (*ClientMQTT::OnMessage)IOT_CALLBACK_SIGNATURE_PARAMETERS
#define MQTT_IOT_CALLBACK_SIGNATURE_MESSAGE void (*OnMessage)IOT_CALLBACK_SIGNATURE_PARAMETERS

#define MESSAGE 0
#define MESSAGE_START 1
#define MESSAGE_END 2

#endif

class ClientMQTT {
  public:   
    ClientMQTT(const char* module) {
      espClient = new WiFiClient();
      client = new PubSubClient(*espClient);
      module_name = ConcatCharArray("ESP8266Client_", module);
      module_server = ConcatCharArray(module, "_server");
      module_client = ConcatCharArray(module, "_client");
      ClientMQTT::message_type = 0;
      ClientMQTT::waiting_response = false;
      ClientMQTT::response_message = NULL;
    }
  
    void ConnectWIFI(const char* ssid, const char* password) {
        if(DEBUG) {
          delay(10);
          Serial.println("");
          Serial.print("Connecting to ");
          Serial.print(ssid);
          Serial.print(" ");  
        }
        WiFi.begin(ssid, password);
        if(DEBUG) {
          while (WiFi.status() != WL_CONNECTED) {
             delay(500);
             Serial.print(".");
          }
          Serial.println(" WiFi connected");
          Serial.print("IP address: ");
          Serial.println(WiFi.localIP());
        }
    }

    void ConnectMQTT(const char* mqtt_server, unsigned int port = 1883, 
      MQTT_IOT_CALLBACK_SIGNATURE_START = NULL, 
      MQTT_IOT_CALLBACK_SIGNATURE_END = NULL, 
      MQTT_IOT_CALLBACK_SIGNATURE_MESSAGE = NULL) {
        client->setServer(mqtt_server, port);
        client->setCallback(ClientMQTT::Callback);
        ClientMQTT::Start = Start;
        ClientMQTT::End = End;
        ClientMQTT::OnMessage = OnMessage;
    }

    //THIS SHOULD BE CALLED REGULARLY TO ALLOW THE CLIET TO
    //PROCCESS INCOMING MESSAGES AND MAINTAIN ITS CONNECTION TO THE SERVER
    void Refresh() {
      client->loop();
    }

    bool StartListening() {
      if (DEBUG) Serial.print("Attempting MQTT connection ... ");  
      if (client->connect(module_name)) {
        if (DEBUG) {
          Serial.print("connected to ");
          Serial.println(module_server);
        }
        client->subscribe(module_server);
      } else if (DEBUG)  {
        Serial.print("failed, rc = ");
        Serial.println(client->state());
      }
      return client->connected();
    }

    bool StartListeningLoop() {
      while (!StartListening()) {
        if (DEBUG) Serial.println("Try again in 5 seconds");
        delay(5000);
      }
    }

    bool IsListening() {
      return client->connected();
    }

    //YOU MUST DELETE THE RETURN (RESPONSE MESSAGE) MEMORY
    char* SendMessageAndWaitForResponse(char * message) {
      SendMessage(message);
      ClientMQTT::waiting_response = true;
      if (DEBUG) Serial.println("WAITING RESPONSE");
      while(ClientMQTT::response_message == NULL) { 
        delay(10);
        client->loop();
      }
      ClientMQTT::waiting_response = false;
      char* response = ClientMQTT::response_message;
      ClientMQTT::response_message = NULL;
      return response;
    }

    void SendMessage(char* message) {
      if (DEBUG) {
        Serial.print("SENDING TO ");
        Serial.print(module_client);
        Serial.print(": ");
        Serial.println(message);
      }
      client->publish(module_client, message);
    }
    
  private:
    char* module_name;
    char* module_client;
    WiFiClient *espClient;
    PubSubClient *client;

    static bool waiting_response;
    static char* response_message; 
    static char* module_server;
    static unsigned int message_type;
    static MQTT_IOT_CALLBACK_SIGNATURE_START;
    static MQTT_IOT_CALLBACK_SIGNATURE_END;
    static MQTT_IOT_CALLBACK_SIGNATURE_MESSAGE;

    static void Callback(char* topic, byte* payload, unsigned int length) {
      if (DEBUG) {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("]: ");
        for (int i = 0; i < length; i++) {
          Serial.print((char)payload[i]);
        }
        Serial.println("");
      }
      if (strcmp(topic, ClientMQTT::module_server) != 0) {
        if (DEBUG) Serial.println("Action: IGNORE");
        return;
      }
      char* message = ByteArrayToCharArray(payload, length);
      if (strcmp(message, "START") == 0) {
        if (DEBUG) Serial.println("Action: IN NEXT MESSAGE EXECUTE Start()");
        ClientMQTT::message_type = MESSAGE_START;
        delete message;
      } else if (strcmp(message, "END") == 0) {
        if (DEBUG) Serial.println("Action: EXECUTE End()");
        ClientMQTT::message_type = MESSAGE_END;
        if (End) End(message);
        delete message;
      } else if (ClientMQTT::message_type == MESSAGE_START) {
          if (DEBUG) Serial.println("Action: EXECUTE Start()");
          if (Start) Start(message);
          delete message;
          ClientMQTT::message_type = MESSAGE;
      } else {//ON MESSAGE
        if (ClientMQTT::waiting_response) {
          if (DEBUG) Serial.println("Action: ASSIGNING RESPONSE");
          ClientMQTT::response_message = message;
        } else {
          if (DEBUG) Serial.println("Action: EXECUTE OnMessaje()");
          if (OnMessage) OnMessage(message);  
          delete message;
        }
        ClientMQTT::message_type = MESSAGE;
      }
    }

    char* ConcatCharArray(const char * str1, const char * str2) {
      int str1L = strlen(str1);
      int str2L = strlen(str2);
      int stringLength = str1L + str2L + 1;
      char* command = new char[stringLength];
      int a = 0;
      for (int i = 0; i < str1L; i++, a++) {
        command[a] = str1[i];
      }
      for (int i = 0; i < str2L; i++, a++) {
        command[a] = str2[i];
      }
      command[a] = '\0';
      return command;
    }

    static char* ByteArrayToCharArray(const byte* payload, unsigned int length) {
      char* message = new char[length+1];
      for (int i = 0; i < length; i++) {
        message[i] = (char)payload[i];
      }
      message[length] = '\0';
      return message;
    }
};
bool ClientMQTT::waiting_response;
char* ClientMQTT::response_message; 
char* ClientMQTT::module_server;
unsigned int ClientMQTT::message_type;
MQTT_IOT_CALLBACK_SIGNATURE_STATIC_START;
MQTT_IOT_CALLBACK_SIGNATURE_STATIC_END;
MQTT_IOT_CALLBACK_SIGNATURE_STATIC_MESSAGE;

////////////////////////////////////////////////////////////////////////////////
const char* ssid = "ASUS";
const char* password = "M#V#23622607";
const char* mqtt_server = "192.168.2.150";
const unsigned int port = 1883;
const char* module = "Wires";
ClientMQTT client(module);
bool started = false;

void f_Start(char* message) {
  Serial.println("HOLI: START");
  started = true;
}

void f_End(char* message) {
  Serial.println("HOLI: END");
  started = false;
}

void f_OnMessage(char* message) {
  Serial.println("HOLI: ON MESSAGE");
}

void setup() {
  Serial.begin(9600);
  client.ConnectWIFI(ssid, password);
  client.ConnectMQTT(mqtt_server, port, f_Start, f_End, f_OnMessage);
}

void loop() {
  if(!client.IsListening()) {
    client.StartListeningLoop();
  }
  client.Refresh();
  delay(1000);
  /*
  Serial.println("SIN ESPERA");
  client.SendMessage("0");
  client.Loop();
  */
  Serial.println("CON ESPERA");
  char* response = client.SendMessageAndWaitForResponse("1");
  Serial.print("RESPUESTA: ");
  Serial.println(response);
  delete response;
  /**/
}
