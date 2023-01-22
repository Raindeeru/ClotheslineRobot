// Motor A
 
int en_A = 9;
int in_1 = 8;
int in_2 = 7;
 
// Motor B
 
int en_B = 3;
int in_3 = 5;
int in_4 = 4;

// Sample using LDR 
int LDRval = A0;

void setup()
 
{

 Serial.begin(9600);
  // Set all the motor control pins to outputs
  pinMode(en_A, OUTPUT);
  pinMode(en_B, OUTPUT);
  pinMode(in_1, OUTPUT);
  pinMode(in_2, OUTPUT);
  pinMode(in_3, OUTPUT);
  pinMode(in_4, OUTPUT);
 
	pinMode(LDRval, INPUT);
}

void loop() {

LDRval = analogRead (A0); // Read LDR value
  Serial.println (LDRval);  // Print LDR value
  Serial.println ("hello");  // Print LDR value
  delay(500);
  if (LDRval <= 50) // insert number here
  // For when the device closes
  //For Anti Clock-wise motion - IN_1 = LOW , IN_2 = HIGH
  {digitalWrite(in_1,LOW) ;
  digitalWrite(in_2,HIGH) ;
  digitalWrite(in_3, LOW) ;
  digitalWrite(in_4, HIGH) ; 
  delay(3000) ;}

  else
  // For when the device opens
  //For Clock-wise motion - IN_1 = HIGH , IN_2 = LOW
  {digitalWrite(in_1,HIGH) ;
  digitalWrite(in_2,LOW) ;
  digitalWrite(in_3, HIGH) ;
  digitalWrite(in_4, LOW) ; 
  delay(3000) ;}


//For brake
digitalWrite(in_1,LOW) ;
digitalWrite(in_2,LOW) ;
digitalWrite(in_3, LOW) ;
digitalWrite(in_4, LOW) ;
delay(1000) ;

}