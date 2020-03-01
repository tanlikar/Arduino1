
void setup (){

  pinMode(3, OUTPUT);
}

void loop(){
  int val = map(analogRead(A0), 0, 1023, 0, 255);
  analogWrite(3, val);
}
