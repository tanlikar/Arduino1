int in1=6;
int in2=7;

int out1= 10;
int out2=11;

void setup() {
  // put your setup code here, to run once:
  pinMode(in1, INPUT);
  pinMode(in2, INPUT);

  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(in1)){
    digitalWrite(out1, HIGH);
  }else{
    digitalWrite(out1, LOW);
  }

  if(digitalRead(in2)){
    digitalWrite(out2, HIGH);
  }else{
    digitalWrite(out2, LOW);
  }
  
}
