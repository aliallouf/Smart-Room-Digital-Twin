#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// --- Network Settings ---
const char* ssid = "Wokwi-GUEST"; 
const char* password = "";
const char* mqtt_server = "broker.hivemq.com"; 

// --- Hardware Pins ---
#define DHTPIN 15
#define DHTTYPE DHT22
#define AC_LED 2  // The "Virtual Air Conditioner"

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

// --- 1. Transmission: Receiving Commands from the Twin ---
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Manual Override from the Cloud Dashboard
  if (message == "ON") { 
    digitalWrite(AC_LED, HIGH); 
    Serial.println("Cloud Command: AC forced ON");
  }
  else if (message == "OFF") { 
    digitalWrite(AC_LED, LOW); 
    Serial.println("Cloud Command: AC forced OFF");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(AC_LED, OUTPUT); // Initialize the AC LED
  dht.begin();
  
  // Connect to Virtual WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); // Link the callback function
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "SmartRoom-Twin-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected to Cloud");
      // IMPORTANT: Resubscribe to control topic upon reconnection
      client.subscribe("myuniqueuser/room1/control"); 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Keeps the MQTT connection alive and checks for new messages

  // --- 2. Data Acquisition: Reading the Room ---
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // --- 3. Synchronization: Update the Cloud Twin ---
  String topic = "myuniqueuser/room1/data"; 
  String payload = "{\"temp\": " + String(t) + ", \"hum\": " + String(h) + "}";
  client.publish(topic.c_str(), payload.c_str());

  // --- 4. Action/Insight: Automatic Local Logic ---
  if (t > 30.0) {
    digitalWrite(AC_LED, HIGH); // The "Twin" brain decides it's too hot
    Serial.println("Local Insight: Temp > 30C. Auto-AC ON.");
  } else {
    // Note: Manual Cloud commands can still turn this off in the next loop
    digitalWrite(AC_LED, LOW); 
  }

  delay(2000); 
}