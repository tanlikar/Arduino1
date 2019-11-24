#include <DS3231.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 6, 7, 8, 9 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 2, 3, 4, 5 };
//  Create the Keypad
  Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
DS3231  rtc(SDA, SCL);
LiquidCrystal lcd(A0, A1, A2, 11, 12, 13); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
 //int angle = 0;    
// int potentio = A0;      // initialize the A0analog pin for potentiometer
 int t1, t2, t3, t4, t5, t6;
 
boolean feed = true; // condition for alarm
 char key;
 int temp[6];
 int time1[6];
 int time2[6];
 int time3[6];

//varaible for non blocking delay
 unsigned long previousMillis = 0; 
 const long interval = 5000;   
 
 void setup() 
 { 
  rtc.begin();
  lcd.begin(16,2);
  
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(0, OUTPUT);
 } 
 
 void loop() 
 { 
lcd.setCursor(0,0);
int buttonPress;
buttonPress = digitalRead(A3);
if (buttonPress==1)
 setFeedingTime();
 
//Serial.println(buttonPress);
 lcd.print("Time:  ");
 String t = "";
 t = rtc.getTimeStr(); 
 t1 = t.charAt(0)-48;
 t2 = t.charAt(1)-48;
 t3 = t.charAt(3)-48;
 t4 = t.charAt(4)-48;
 t5 = t.charAt(6)-48;
 t6 = t.charAt(7)-48;
 

 lcd.print(rtc.getTimeStr());
 lcd.setCursor(0,1);
 lcd.print("Date: ");
 lcd.print(rtc.getDateStr());
 
 if ((t1==time1[0] && t2==time1[1] && t3==time1[2] && t4==time1[3]&& t5<1 && t6<3)|| (t1==time2[0] && t2==time2[1] && t3==time2[2] && t4==time2[3]&& t5<1 && t6<3) || (t1==time3[0] && t2==time3[1] && t3==time3[2] && t4==time3[3]&& t5<1 && t6<3) )
 { 
    digitalWrite(1, HIGH);
    digitalWrite(0, LOW);
 } 

//non blocking delay
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {

     previousMillis = currentMillis;
     digitalWrite(1, LOW);
     digitalWrite(0, LOW);
   }
 
 }       
void setFeedingTime()
{
  feed = true;
   int i=0;
   int j=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set feeding Time");
  delay(500);
  lcd.clear();
  lcd.print("HH:MM");
  lcd.setCursor(0,1);
  
  while(1){
    key = kpd.getKey();
    
  if(key!=NO_KEY){
    
    lcd.setCursor(j,1);
    
    lcd.print(key);
    
    temp[i] = key-48;
    i++;
    j++;
    if (j==2)
    {
      lcd.print(":"); j++;
    }
    delay(500);
  }
  if (key == 'D')
  {
    for(int x = 0; x<6; x++){
      time1[x] = temp[x];
    }
    key=0;
    j=0;
    break;
    }
   if (key == 'C')
  {
    for(int x = 0; x<6; x++){
      time2[x] = temp[x];
    }
    key=0;
    j=0;
    break;
    }
   if (key == 'B')
  {
    for(int x = 0; x<6; x++){
      time3[x] = temp[x];
    }
    key=0;
    j=0;
    break;
    }
   
  }
}
