// Motor A connections
int enA = 9; //Speed Control A;
int in1 = 8; //Turn this on, the motor spins forward
int in2 = 7; //Turn this on, the motor spins backward
// Motor B connections
int enB = 3; //Speed Control B
int in3 = 5; //Turn this on, the motor spins forward
int in4 = 4; //Turn this on the motor spins backward

void setup()
{
    Serial.begin(9600);

	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

    delay (2000);

    digitalWrite(in1, HIGH);
}

void loop()
{
	
}