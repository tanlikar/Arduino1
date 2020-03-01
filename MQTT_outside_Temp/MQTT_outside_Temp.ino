#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DHTPIN D3     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

const char* ssid = "tan";
const char* password = "123456789";
const char* mqtt_server = "192.168.137.158";
const char* clientID = "NodeMCUDevKit3";

const char* outTopic1 = "DHT22out"; //nodemcu send out

//intopic esp8266 mqtt input
const char* inTopic1 = "intervalAPI"; //nodemcu receive

 // Values read from sensor
float temp_c; 
String temp;
char cTemp[50];

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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

//mqtt send
void callback(char* topic, byte* payload, unsigned int length) {
  // Conver the incoming byte array to a string
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char*)payload;
  
  Serial.print("Message arrived on topic: [");
  Serial.print(topic);// received topic
  Serial.print("], ");
  Serial.println(message); // recived payload

if(strcmp(topic,inTopic1)==0){ // strcmp return 0 if true

    temp = String(temp_c);
    temp.toCharArray(cTemp, 50);
    client.publish(outTopic1, cTemp);
    temp_c = NAN;
}

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(outTopic, clientID);
      // ... and resubscribe
      client.subscribe(inTopic1);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  dht.begin();

}

int period = 1000;
unsigned long time_now = 0;

void loop() {

   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // put your main code here, to run repeatedly:
  if(millis()> time_now + period){
   time_now = millis();
   temp_c = dht.readTemperature();    // Read temperature as Celcius
   Serial.println(temp_c);
  }

}
