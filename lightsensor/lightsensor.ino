int ldr100 = A0;
int ldr500 = A1;
int ldr1k = A2;

float ldr100val;
float ldr500val;
float ldr1kval;

void setup()
{
    Serial.begin(9600);
    pinMode(ldr100, INPUT);
    pinMode(ldr500, INPUT);
    pinMode(ldr1k, INPUT);
}

void loop()
{
    ldr100val = analogRead(ldr100)/204.8;
    ldr500val = analogRead(ldr500)/204.8;
    ldr1kval = analogRead(ldr1k)/204.8;

    Serial.println("___________");
    Serial.println(ldr100val);
    Serial.println(ldr500val);
    Serial.println(ldr1kval);

    delay(1000);
}