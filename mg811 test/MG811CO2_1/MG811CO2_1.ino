/* Sending Sensor Data to Firebase Database */

#include <ESP8266WiFi.h>                                                  // esp8266 library
#include <FirebaseArduino.h>                                             // firebase library
#include <MG811.h>

#define FIREBASE_HOST "iot-kids-monitoring-system.firebaseio.com"       // the project name address from firebase id
#define FIREBASE_AUTH "3FenGbA2QFIRqT5bARfjThSa5PjHm1EsAb"        // the secret key generated from firebase

#define WIFI_SSID "Why So Serious?"                                     // input your home or public wifi name 
#define WIFI_PASSWORD "c15teeaiai203"                                   //password of wifi ssid
 
#define CO2level = 0;
float ppm = 0.0;

MG811 mySensor = MG811(A0); // Analog input A0
float v400 = 4.535;  //sensor voltage output at 400ppm NEED TO BE CHANGE
float v40000 = 3.206; //sensor analog voltage output at 40000ppm  NEED TO BE CHANGE

void setup() {
  Serial.begin(9600);
  mySensor.begin(v400, v40000);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                        //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                          // connect to firebase
}

void loop() { 
                               
  float voltage = mySensor.raw();
  float concentration = mySensor.read();

    Serial.print("voltage:");
    Serial.print(voltage);
    Serial.println("V");
 
    Serial.print(concentration);
    Serial.println("ppm");
  //  Serial.print("Carbon Dioxide: ");  Serial.print(c);
    String fireCO2 = String(concentration);                                         
 // Serial.println(" ppm ");
  
  delay(4000);
  
  Firebase.pushString ("/MG811/Carbon Dioxide", fireCO2);                 //setup path and send readings
     
}
