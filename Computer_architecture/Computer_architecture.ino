int OP_0= A4;  //LSB
int OP_1 = A5; //MSB

int MUX2to1_2to1_1 = 2;  //between reg and ALU
int MUX2to1_2 = 3; // output of ALU

int RT_WE = 4;
int RT_OE = 5;
int RT_CS = 6;

int RS_WE = 7;
int RS_OE = 8;
int RS_CS = 9;

int RD_WE = 10;
int RD_OE = 11;
int RD_CS = 12;

int MEM_WE = 13;
int MEM_OE = A0;
int MEM_CS = A1;

int S1 = A2; // S0, S3
int S2 = A3; // S1, S2

void setup() {

  pinMode(OP_0, INPUT);
  pinMode(OP_1, INPUT);
  
  pinMode(MUX2to1_2to1_1, OUTPUT);
  pinMode(MUX2to1_2, OUTPUT);
  
  pinMode(RT_WE, OUTPUT);
  pinMode(RT_OE, OUTPUT);
  pinMode(RT_CS, OUTPUT);

  pinMode(RS_WE, OUTPUT);
  pinMode(RS_OE, OUTPUT);
  pinMode(RS_CS, OUTPUT);
  
  pinMode(RD_WE, OUTPUT);
  pinMode(RD_OE, OUTPUT);
  pinMode(RD_CS, OUTPUT);

  pinMode(MEM_WE, OUTPUT);
  pinMode(MEM_OE, OUTPUT);
  pinMode(MEM_CS, OUTPUT);

  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int temp0 =  digitalRead(OP_1);
  int temp1 =  digitalRead(OP_0);
  Serial.print(temp1);
  Serial.println(temp0);
      
  if((temp0 == 0) && (temp1 == 0)){ // ADDITION
    Serial.println("addition");
    digitalWrite(MUX2to1_2to1_1, LOW);
    digitalWrite(MUX2to1_2, LOW);
    
    digitalWrite(RT_WE, HIGH);
    digitalWrite(RT_OE, LOW);
    digitalWrite(RT_CS, LOW);
    
    digitalWrite(RS_WE, HIGH);
    digitalWrite(RS_OE, LOW);
    digitalWrite(RS_CS, LOW);

    digitalWrite(RD_WE, LOW);
    digitalWrite(RD_OE, LOW);
    digitalWrite(RD_CS, LOW);

    digitalWrite(MEM_WE, LOW);
    digitalWrite(MEM_OE, LOW);
    digitalWrite(MEM_CS, HIGH);

    digitalWrite(S1, HIGH);
    digitalWrite(S2, LOW);
  }else if((temp0 == 1) && (temp1 == 0)){ //SUBTRACTION

    Serial.println("SUBTRACTION");
    digitalWrite(MUX2to1_2to1_1, LOW);
    digitalWrite(MUX2to1_2, LOW);
    
    digitalWrite(RT_WE, HIGH);
    digitalWrite(RT_OE, LOW);
    digitalWrite(RT_CS, LOW);
    
    digitalWrite(RS_WE, HIGH);
    digitalWrite(RS_OE, LOW);
    digitalWrite(RS_CS, LOW);

    digitalWrite(RD_WE, LOW);
    digitalWrite(RD_OE, LOW);
    digitalWrite(RD_CS, LOW);

    digitalWrite(MEM_WE, LOW);
    digitalWrite(MEM_OE, LOW);
    digitalWrite(MEM_CS, HIGH);

    digitalWrite(S1, LOW);
    digitalWrite(S2, HIGH);
  }else if((temp0 == 0) && (temp1 == 1){ //store

    Serial.println("store");
    
    digitalWrite(MUX2to1_2to1_1, HIGH);
    digitalWrite(MUX2to1_2, HIGH);
    
    digitalWrite(RT_WE, HIGH);
    digitalWrite(RT_OE, LOW);
    digitalWrite(RT_CS, LOW);
    
    digitalWrite(RS_WE, HIGH);
    digitalWrite(RS_OE, LOW);
    digitalWrite(RS_CS, LOW);

    digitalWrite(RD_WE, LOW);
    digitalWrite(RD_OE, LOW);
    digitalWrite(RD_CS, HIGH);

    digitalWrite(MEM_WE, LOW);
    digitalWrite(MEM_OE, LOW);
    digitalWrite(MEM_CS, LOW);

    digitalWrite(S1, HIGH);
    digitalWrite(S2, LOW);
  }
}
