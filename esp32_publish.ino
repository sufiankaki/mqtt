#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* WiFi_ssid = "";
const char* WiFi_password = "";

const char* mqtt_server = "iot.atria.edu";
int mqtt_port = 1883;

const char* subtopic = "led";
const char* pubtopic = "monitor";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WiFi_ssid);

  WiFi.begin(WiFi_ssid, WiFi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(subtopic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32")) {
      Serial.println("connected");
      client.subscribe(subtopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  StaticJsonDocument<200> values;
  values["test"] = 1;
  
  char jsonBuffer[512];
  serializeJson(values, jsonBuffer);
  client.publish(pubtopic, jsonBuffer);

  Serial.println(jsonBuffer);
  delay(1000);
}


/* Integrate the following program above

#include "DHT.h"

#define DHTPIN 2     
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();
}

void loop() {
  delay(2000);
  Serial.print(F("Humidity: "));
  Serial.print(dht.readHumidity());
  Serial.print(F("%  Temperature: "));
  Serial.print(dht.readTemperature());
  Serial.print(F("°C "));
}

*/
