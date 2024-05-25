#include <ezButton.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define BUTTON_PIN 5  
#define LED_PIN 23    

const char* ssid = "MEDIOTTI";                 
const char* password = "gabi250023";           
const char* mqtt_server = "mqtt-dashboard.com";  
const char* mqtt_topic = "mediotti/room/button";  

WiFiClient espClient;
PubSubClient client(espClient);
ezButton button(BUTTON_PIN);  


int led_state = LOW;  

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);          
  pinMode(LED_PIN, OUTPUT);    
  button.setDebounceTime(50);  

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  button.loop();  

  if (button.isPressed()) {
    Serial.println("The button is pressed");

    
    led_state = !led_state;

    
    digitalWrite(LED_PIN, led_state);

    
    client.publish(mqtt_topic, "Button Pressed");
  }
}
