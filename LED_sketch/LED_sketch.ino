
const int red1 = 2, red2 = 5, red3 = 7, red4 = 9, red5 = 11, red6 = 13;
const int blue1 = 3, blue2 = 4, blue3 = 6, blue4 = 8, blue5 = 10, blue6 = 12;


void setup() {
  
  // put your setup code here, to run once:
  pinMode(red1,OUTPUT);
  pinMode(red2,OUTPUT);
  pinMode(red3,OUTPUT);
  pinMode(red4,OUTPUT);
  pinMode(red5,OUTPUT);
  pinMode(red6, OUTPUT);
  pinMode(blue1,OUTPUT);
  pinMode(blue2,OUTPUT);
  pinMode(blue3,OUTPUT);
  pinMode(blue4,OUTPUT);
  pinMode(blue5,OUTPUT);
  pinMode(blue6, OUTPUT);
  digitalWrite(red1, HIGH);
  delay(100);
  digitalWrite(red2, HIGH);
  delay(100);
  digitalWrite(red3, HIGH);
  delay(100);
  digitalWrite(red4, HIGH);
  delay(100);
  digitalWrite(red5, HIGH);
  delay(100);
  digitalWrite(red6, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(red1,LOW);
  digitalWrite(blue1,HIGH);
  delay(100);
  digitalWrite(red2,LOW);
  digitalWrite(blue2,HIGH);
  delay(100);
  digitalWrite(red3,LOW);
  digitalWrite(blue3,HIGH);
  delay(100);
  digitalWrite(red4,LOW);
  digitalWrite(blue4,HIGH);
  delay(100);
  digitalWrite(red5,LOW);
  digitalWrite(blue5,HIGH);
  delay(100);
  digitalWrite(red6, LOW);
  digitalWrite(blue6, HIGH);
  delay(100);
  digitalWrite(red1,HIGH);
  digitalWrite(blue1,LOW);
  delay(100);
  digitalWrite(red2,HIGH);
  digitalWrite(blue2,LOW);
  delay(100);
  digitalWrite(red3,HIGH);
  digitalWrite(blue3,LOW);
  delay(100);
  digitalWrite(red4,HIGH);
  digitalWrite(blue4,LOW);
  delay(100);
  digitalWrite(red5,HIGH);
  digitalWrite(blue5,LOW);
  delay(100);
  digitalWrite(red6, HIGH);
  digitalWrite(blue6, LOW);
  delay(100);
  
}
