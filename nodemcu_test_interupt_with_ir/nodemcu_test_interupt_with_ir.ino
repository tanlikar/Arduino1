#include <ESP8266WiFi.h>
#include <PubSubClient.h>


WiFiClient espClient;
PubSubClient client(espClient);


const int interruptBtn1 = D7;
const int interruptBtn2 = D8;
const int interruptIR1 = D5;
const int interruptIR2 = D6;

const int motorAcon = D1; //sd2
const int motorBcon = D2;  //sd3

//i2c use pin D3, D4


const int item1point = 400;
const int item2point = 500;

volatile int motorAflag = 0;
volatile int motorBflag = 0;
volatile int transaction = 0;


const char* ssid = "tan";
const char* password = "123456789";
const char* mqtt_server = "192.168.137.159";
const char* clientID = "makerthon";


const char* outTopic1 = "item1out"; //nodemcu send out
const char* outTopic2 = "item2out"; //nodemcu send out

//intopic esp8266 mqtt input
const char* inTopic1 = "item1in"; //nodemcu receive
const char* inTopic2 = "item2in"; //nodemcu receive
const char* inTopic3 = "transcationIn";

void ICACHE_RAM_ATTR motorArun() {
 motorAflag = 1;
}

void ICACHE_RAM_ATTR motorBrun() {
 motorBflag = 1;
}

void ICACHE_RAM_ATTR motorAstop() {
 motorAflag = 0;
}

void ICACHE_RAM_ATTR motorBstop() {
 motorBflag = 0;
}

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
      client.subscribe(inTopic2);
      client.subscribe(inTopic3);
      
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
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(interruptBtn1, INPUT_PULLUP);
  pinMode(interruptBtn2, INPUT_PULLUP);
  pinMode(interruptIR1, INPUT);
  pinMode(interruptIR2, INPUT);

  pinMode(motorAcon, OUTPUT);
  pinMode(motorBcon, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(interruptBtn1), motorArun, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptBtn2), motorBrun, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptIR1), motorAstop, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptIR2), motorBstop, CHANGE);

}

void loop() {
  // put your main code here, to run repeatedly:

   if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(motorAflag == 1){
    digitalWrite(motorAcon, HIGH);
  }else{
    digitalWrite(motorAcon, LOW);
  }

  if(motorBflag == 1){
    digitalWrite(motorBcon, HIGH);
  }else{
    digitalWrite(motorBcon, LOW);
  }
  

}
