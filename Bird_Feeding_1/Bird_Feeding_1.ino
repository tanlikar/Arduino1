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
 int r[6];
 
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
 
 if (t1==r[0] && t2==r[1] && t3==r[2] && t4==r[3]&& t5<1 && t6<3 && feed==true)
 { 
    digitalWrite(1, HIGH);
    digitalWrite(0, LOW);
    delay(5000);
    digitalWrite(1, LOW);
    digitalWrite(0, LOW);
    feed=false; 
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
    
    r[i] = key-48;
    i++;
    j++;
    if (j==2)
    {
      lcd.print(":"); j++;
    }
    delay(500);
  }
  if (key == 'D')
  {key=0; j=0; break;}
  }
}
