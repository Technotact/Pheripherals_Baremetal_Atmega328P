#include <SoftwareSerial.h>
#include <ThingSpeak.h>

#define RX 2
#define TX 3

String WIFI_SSID = "JyotiElab";
String WIFI_PASS = "9206299326";

String PORT = "80";

char* HOST = "api.thingspeak.com";
unsigned long channelID = 2785873;
char* API = "D5QPIW2F4E29KQS5";
unsigned int dataFieldOne = 1;

SoftwareSerial esp01(RX, TX);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  esp01.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + WIFI_SSID + "\",\"" + WIFI_PASS + "\"", 20, "OK");
}

void loop() {
  String httpRequest = "GET /channels/2785873/fields/1.json?results=1 HTTP/1.1\r\n";
  httpRequest += "Host: api.thingspeak.com\r\n";
  httpRequest += "Connection: close\r\n\r\n";

  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(httpRequest.length()), 4, ">");
  esp01.print(httpRequest); // Send the HTTP request
  delay(2000);

  // Read the response
  String response = readResponse();

  // Extract JSON from response
  int jsonStart = response.indexOf("\r\n\r\n"); // Find start of JSON (after HTTP headers)
  if (jsonStart != -1) {
    String jsonData = response.substring(jsonStart + 4); // Extract JSON part

    // Parse the JSON data
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, jsonData);

    if (error) {
      Serial.print("JSON parsing failed: ");
      Serial.println(error.c_str());
    } else {
      // Extract the value of field1
      const char* field1 = doc["feeds"][0]["field1"];
      Serial.print("Value of field1: ");
      Serial.println(field1);
    }
  } else {
    Serial.println("Failed to extract JSON from the response.");
  }

  sendCommand("AT+CIPCLOSE=0", 5, "OK");
  delay(20000);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(". AT command => ");
  Serial.print(command);
  while (maxTime--) {
    esp01.println(command);
    if (esp01.find(readReplay)) {
      Serial.println("OK");
      return;
    }
  }
  Serial.println("Fail");
}

String readResponse() {
  String response = "";
  while (esp01.available()) {
    char c = esp01.read();
    response += c;
  }
  Serial.println("Response from ESP-01:");
  Serial.println(response);
  return response;
}
