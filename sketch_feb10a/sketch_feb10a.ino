void setup(){
  pinMode(3, OUTPUT); // use pin 3 for square wave
}

void loop(){

  unsigned int duration = 1000; // dont know how long is this, experiment for yourself

  for(int x = 0 ; x < duration; x++){
    digitalWrite(3, HIGH);
    delay(5);
    digitalWrite(3, LOW);
    delay(5);
  }
}
