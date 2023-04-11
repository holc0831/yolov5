#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";//replace this with your wifi access point 
//name
const char* password = ""; //replace with your wifi password
const char* broker = ""; //IP address of broker
const int port = 1883;
const char* mqttUser = "user";
const char* mqttPassword = "user";


const int RELAY_PIN = 5;  // the Arduino pin, which connects to the IN pin of relay

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received");
//  Serial.print(topic);
//  Serial.print("   length is:");
//  Serial.println(length);
//  Serial.print("Data Received From Broker:");
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
//
//  }
  digitalWrite(RELAY_PIN, HIGH);
  delay(8000);
//  client.setServer(broker, port);
//  client.subscribe("cv2x");\
  
  //ESP.restart();
}

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    yield();
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(broker, port);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("connected to MQTT broker");
      pinMode(RELAY_PIN, OUTPUT);
    }
    else
    {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(500);
    }
  }
  
  Serial.println("ESP8266 AS SUBSCRIBER");
  Serial.println("Subscribing topic test:");
  client.subscribe("cv2x");//topic name="abc"
  // Register event handlers for Wi-Fi events
  WiFi.onStationModeDisconnected(onStationDisconnected);
  WiFi.onStationModeGotIP(onStationGotIP);
}

void loop() {
  if (!client.connected())  // Reconnect if connection is lost
  {
    reconnect();
  }
  client.loop();
  digitalWrite(RELAY_PIN, LOW);
  
  
}
//  client.loop();
//  delay(100);
//  Serial.print("ping");

  



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if(client.connect("ESP32Client", mqttUser, mqttPassword )) {
      client.subscribe("cv2x");
      Serial.println("connected");

    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void onStationDisconnected(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi network");
  Serial.print("Reason: ");
  Serial.println(event.reason);
  
  // Attempt to reconnect to Wi-Fi network
  WiFi.begin(ssid, password);
}

void onStationGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.print("Connected to Wi-Fi network. IP address: ");
  Serial.println(WiFi.localIP());
}
