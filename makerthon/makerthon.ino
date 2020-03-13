#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient espClient;
PubSubClient client(espClient);

const int interruptBtn1 = D3;
const int interruptBtn2 = D4;
const int IR1 = D5;
const int IR2 = D6;

const int motorAcon = D7; 
const int motorBcon = D8;  

//i2c use pin D1 = SCL, D2 = SDA
const int i2cSDA = D2;
const int i2cSCL = D1;

const int item1point = 400;
const int item2point = 500;

volatile int motorAflag = 0;
volatile int motorBflag = 0;

volatile int buttonStateIR1;             // the current reading from the input pin
volatile int lastbuttonStateIR1 = LOW;   // the previous reading from the input pin

volatile int buttonStateIR2;             // the current reading from the input pin
volatile int lastbuttonStateIR2 = LOW;   // the previous reading from the input pin

volatile unsigned long lastDebounceTimeIR1 = 0;  // the last time the output pin was toggled
volatile unsigned long lastDebounceTimeIR2 = 0;  // the last time the output pin was toggled
volatile unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

volatile int itemPrice = 0;
volatile int userPaid = 0;

volatile unsigned int item1Stock = 0;
volatile unsigned int item2Stock = 0;

volatile int paidItem1Flag = 0;
volatile int paidItem2Flag = 0;

volatile int item1SelectFlag = 0;
volatile int item2SelectFlag = 0;

const char* ssid = "tan";
const char* password = "123456789";
const char* mqtt_server = "192.168.137.159";
const char* clientID = "makerthon";


const char* outTopic1 = "item1out"; //nodemcu send out
const char* outTopic2 = "item2out"; //nodemcu send out

//intopic esp8266 mqtt input
const char* inTopic1 = "item1in"; //nodemcu receive
const char* inTopic2 = "item2in"; //nodemcu receive
const char* inTopic3 = "paymentIn";

void ICACHE_RAM_ATTR item1Select() { 
 itemPrice = item1point;
 item1SelectFlag = 1;
 item2SelectFlag = 0;

 lcd.clear();
 lcd.home();
 lcd.print("Item1 Selected");
 lcd.setCursor(0, 1);
 lcd.print("Pay: 400");
}

void ICACHE_RAM_ATTR item2Select() {
 itemPrice = item2point;
 item1SelectFlag = 0;
 item2SelectFlag = 1;

 lcd.clear();
 lcd.home();
 lcd.print("Item2 Selected");
 lcd.setCursor(0, 1);
 lcd.print("Pay: 500");
}

//TODO add motor run

void ICACHE_RAM_ATTR motorAstop() { 
 //motorAflag = 0;
}

void ICACHE_RAM_ATTR motorBstop() {
 //motorBflag = 0;
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
    item1Stock =  message.toInt();
}

if(strcmp(topic,inTopic2)==0){ // strcmp return 0 if true
    item2Stock =  message.toInt();
}

if(strcmp(topic,inTopic3)==0){ // strcmp return 0 if true
    userPaid = message.toInt();

    if(userPaid == itemPrice && item1SelectFlag == 1){
       paidItem1Flag = 1;
       motorAflag = 1;
    }else if(userPaid == itemPrice && item2SelectFlag == 1){
       paidItem2Flag = 1;
       motorBflag = 1;
    }
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
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  pinMode(motorAcon, OUTPUT);
  pinMode(motorBcon, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(interruptBtn1), item1Select, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptBtn2), item2Select, CHANGE);

  Wire.begin(D2, D1);
  lcd.begin();
  lcd.backlight();
  lcd.home();
  lcd.print("Welcome,");
  lcd.setCursor(0,1);
  lcd.print("Select Product");
}

void SendMsg(String loTopic, int loMsg)
{
  String msgToSend= String(loMsg);
  String topicToSend=loTopic;
  if (client.publish((char*) topicToSend.c_str(), (char*) msgToSend.c_str()) )
  {
    Serial.println("Publish Succeeded!");
  }
    else
  {
    Serial.println("Publish Failed!");
  }
}


void IR1debounce(){
  int reading = digitalRead(IR1);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastbuttonStateIR1) {
    // reset the debouncing timer
    lastDebounceTimeIR1 = millis();
  }

  if ((millis() - lastDebounceTimeIR1) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonStateIR1) {
      buttonStateIR1 = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonStateIR1 == HIGH) {
        motorAflag = 0;
        lcd.clear();
        lcd.home();
        lcd.print("Welcome,");
        lcd.setCursor(0,1);
        lcd.print("Select Product");
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastbuttonStateIR1IR1:
  lastbuttonStateIR1 = reading;
}



void IR2debounce(){
  int reading = digitalRead(IR2);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastbuttonStateIR2) {
    // reset the debouncing timer
    lastDebounceTimeIR2 = millis();
  }

  if ((millis() - lastDebounceTimeIR2) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonStateIR2) {
      buttonStateIR2 = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonStateIR2 == HIGH) {
        motorBflag = 0;
        lcd.clear();
        lcd.home();
        lcd.print("Welcome,");
        lcd.setCursor(0,1);
        lcd.print("Select Product");
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastbuttonStateIR1IR1:
  lastbuttonStateIR2 = reading;
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

  if(paidItem1Flag == 1){
    paidItem1Flag = 0;
    item1Stock -- ;
    userPaid = 0;
    SendMsg(outTopic1, item1Stock);
    lcd.clear();
    lcd.home();
    lcd.print("Thank you");
  }

  
  if(paidItem2Flag == 1){
    paidItem2Flag = 0;
    item2Stock -- ;
    userPaid = 0;
    SendMsg(outTopic2, item2Stock);
    lcd.clear();
    lcd.home();
    lcd.print("Thank you");
  }

  Serial.println(motorAflag);

  IR1debounce();
  IR2debounce();


}
