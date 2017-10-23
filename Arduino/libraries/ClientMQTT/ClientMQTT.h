/*
ClientMQTT.h - Library for a MQTT Client.
Created by Santiago Mendez, Bruno Ferrari, Diego Zuluaga
*/

#ifndef ClientMQTT_h
#define ClientMQTT_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string>
using namespace std;

#ifndef DEBUG
#define DEBUG true
#endif

#ifndef CLIENT_MQTT_IOT
#define CLIENT_MQTT_IOT
#define IOT_CALLBACK_SIGNATURE_PARAMETERS (char*)
#define MQTT_IOT_CALLBACK_SIGNATURE_START void (*start)IOT_CALLBACK_SIGNATURE_PARAMETERS
#define MQTT_IOT_CALLBACK_SIGNATURE_END void (*end)IOT_CALLBACK_SIGNATURE_PARAMETERS
#define MQTT_IOT_CALLBACK_SIGNATURE_MESSAGE void (*onMessage)IOT_CALLBACK_SIGNATURE_PARAMETERS
#define MESSAGE 0
#define MESSAGE_START 1
#define MESSAGE_END 2
#endif

class ClientMQTT {
public:
	ClientMQTT(const char* module) {
		espClient = new WiFiClient();
		client = new PubSubClient(*espClient);
		module_name = concatCharArray("ESP8266Client_", module);
		module_server = concatCharArray(module, "_server");
		module_client = concatCharArray(module, "_client");
		message_type = 0;
		waiting_response = false;
		response_message = NULL;
	}

	void connectWIFI(const char* ssid, const char* password) {
		if (DEBUG) {
			delay(10);
			Serial.println("");
			Serial.print("Connecting to ");
			Serial.print(ssid);
			Serial.print(" ");
		}
		WiFi.begin(ssid, password);
		if (DEBUG) {
			while (WiFi.status() != WL_CONNECTED) {
				delay(500);
				Serial.print(".");
			}
			Serial.println(" WiFi connected");
			Serial.print("IP address: ");
			Serial.println(WiFi.localIP());
		}
	}

	void connectMQTT(const char* mqtt_server, unsigned int port = 1883,
		MQTT_IOT_CALLBACK_SIGNATURE_START = NULL,
		MQTT_IOT_CALLBACK_SIGNATURE_END = NULL,
		MQTT_IOT_CALLBACK_SIGNATURE_MESSAGE = NULL) {
		client->setServer(mqtt_server, port);
		client->setCallback([=](char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
		this->start = start;
		this->end = end;
		this->onMessage = onMessage;
	}

	//THIS SHOULD BE CALLED REGULARLY TO ALLOW THE CLIET TO
	//PROCCESS INCOMING MESSAGES AND MAINTAIN ITS CONNECTION TO THE SERVER
	void refresh() {
		client->loop();
	}

	bool startListening() {
		if (DEBUG) Serial.print("Attempting MQTT connection ... ");
		if (client->connect(module_name)) {
			if (DEBUG) {
				Serial.print("connected to ");
				Serial.println(module_server);
			}
			client->subscribe(module_server);
		}
		else if (DEBUG) {
			Serial.print("failed, rc = ");
			Serial.println(client->state());
		}
		return client->connected();
	}

	bool startListeningLoop() {
		while (!startListening()) {
			if (DEBUG) Serial.println("Try again in 5 seconds");
			delay(5000);
		}
	}

	bool isListening() {
		return client->connected();
	}

	//YOU MUST DELETE THE RETURN (RESPONSE MESSAGE) MEMORY
	char* sendMessageAndWaitForResponse(const char * message) {
		sendMessage(message);
		waiting_response = true;
		if (DEBUG) Serial.println("WAITING RESPONSE");
		while (response_message == NULL) {
			delay(10);
			client->loop();
		}
		waiting_response = false;
		char* response = response_message;
		response_message = NULL;
		return response;
	}

	void sendMessage(const char* message) {
		if (DEBUG) {
			Serial.print("SENDING TO ");
			Serial.print(module_client);
			Serial.print(": ");
			Serial.println(message);
		}
		client->publish(module_client, message);
	}

	~ClientMQTT() {
		delete module_name, module_client;
		delete client, espClient;
		if (module_server != NULL)
			delete module_server;
		if (response_message != NULL)
			delete response_message;
	}

private:
	char* module_name;
	char* module_client;
	WiFiClient *espClient;
	PubSubClient *client;
	bool waiting_response;
	char* response_message;
	char* module_server;
	unsigned int message_type;
	MQTT_IOT_CALLBACK_SIGNATURE_START;
	MQTT_IOT_CALLBACK_SIGNATURE_END;
	MQTT_IOT_CALLBACK_SIGNATURE_MESSAGE;

	void callback(char* topic, byte* payload, unsigned int length) {
		if (DEBUG) {
			Serial.print("Message arrived [");
			Serial.print(topic);
			Serial.print("]: ");
			for (int i = 0; i < length; i++) {
				Serial.print((char)payload[i]);
			}
			Serial.println("");
		}
		if (strcmp(topic, module_server) != 0) {
			if (DEBUG) Serial.println("Action: IGNORE");
			return;
		}
		char* message = byteArrayToCharArray(payload, length);
		if (strcmp(message, "START") == 0) {
			if (DEBUG) Serial.println("Action: IN NEXT MESSAGE EXECUTE Start()");
			message_type = MESSAGE_START;
			delete message;
		}
		else if (strcmp(message, "END") == 0) {
			if (DEBUG) Serial.println("Action: EXECUTE End()");
			message_type = MESSAGE_END;
			if (end) end(message);
			delete message;
		}
		else if (message_type == MESSAGE_START) {
			if (DEBUG) Serial.println("Action: EXECUTE Start()");
			if (start) start(message);
			delete message;
			message_type = MESSAGE;
		}
		else {//ON MESSAGE
			if (waiting_response) {
				if (DEBUG) Serial.println("Action: ASSIGNING RESPONSE");
				response_message = message;
			}
			else {
				if (DEBUG) Serial.println("Action: EXECUTE OnMessage()");
				if (onMessage) onMessage(message);
				delete message;
			}
			message_type = MESSAGE;
		}
	}

	static char* concatCharArray(const char * str1, const char * str2) {
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

	static char* byteArrayToCharArray(const byte* payload, unsigned int length) {
		char* message = new char[length + 1];
		for (int i = 0; i < length; i++) {
			message[i] = (char)payload[i];
		}
		message[length] = '\0';
		return message;
	}
};
#endif
