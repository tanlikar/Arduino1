/* Sending Sensor Data to Firebase Database */

#include <ESP8266WiFi.h>                                                  // esp8266 library
#include <FirebaseArduino.h>                                             // firebase library

#define FIREBASE_HOST "iot-kids-monitoring-system.firebaseio.com"       // the project name address from firebase id
#define FIREBASE_AUTH "3FenGbA2QFIRqT5bARfjThSa5PjHm1EsAb"        // the secret key generated from firebase

#define WIFI_SSID "Why So Serious?"                                     // input your home or public wifi name 
#define WIFI_PASSWORD "c15teeaiai203"                                   //password of wifi ssid
 
#define CO2level = 0;
float ppm = 0.0;

/************************Hardware Related Macros************************************/
#define         MG_PIN                       (A0)     //define which analog input channel you are going to use
#define         BOOL_PIN                     (D0)
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier
 
/***********************Software Related Macros************************************/
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interval(in milisecond) between each samples in 
                                                     //normal operation
 
/**********************Application Related Macros**********************************/ //you need to calibrate this for accurate reading
//These two values differ from sensor to sensor. user should derermine this value.
#define         ZERO_POINT_VOLTAGE           (0.220) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         REACTION_VOLTGAE             (0.020) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2
 
/*****************************Globals***********************************************/ 
float           CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};   //you need to calibrate this for accurate reading
                                                     //two points are taken from the curve. 
                                                     //with these two points, a line is formed which is
                                                     //"approximately equivalent" to the original curve.
                                                     //data format:{ x, y, slope}; point1: (lg400, 0.324), point2: (lg4000, 0.280) 
                                                     //slope = ( reaction voltage ) / (log400 –log1000) 

void setup() {
  Serial.begin(9600);
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
                               
  float voltage = MGRead(MG_PIN);
  float concentration = MGGetPercentage(voltage, CO2Curve);

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

float MGRead(int mg_pin)
{
    int i;
    float v=0;
 
    for (i=0;i<READ_SAMPLE_TIMES;i++) {
        v += analogRead(mg_pin);
        delay(READ_SAMPLE_INTERVAL);
    }
    v = (v/READ_SAMPLE_TIMES) *3.3/1024 ;  //
    return v;  
}

int  MGGetPercentage(float volts, float *pcurve)
{
   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
      return -1;
   } else { 
      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
   }
}
