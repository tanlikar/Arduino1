const int pwmPin = 6;

void setup()
{
// initialize serial communication at 9600 bits per second:
  pinMode(pwmPin, OUTPUT);
  
}

void loop()
{
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);

  float voltage = map(sensorValue, 0, 1023, 0, 255);

  analogWrite(pwmPin, voltage);
}
