const int ledPin = D7;
const int interruptPin = D1;
volatile int state = LOW;


void ICACHE_RAM_ATTR blink() {
  state = !state;
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);
}

void loop() {
  digitalWrite(ledPin, state);
  Serial.println(state);
  delay(10);
  
}
