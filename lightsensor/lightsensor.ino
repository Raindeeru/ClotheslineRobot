int ldr100 = A0;
int ldr500 = A1;
int ldr1k = A2;

float ldr100val;
float ldr500val;
float ldr1kval;

float irradiance;

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

    //I know this code looks ugly, sorry :(
    if(2.75 < ldr1kval && ldr1kval < 4.76)
    {
        irradiance = G1(ldr1kval);
        Serial.println("twas me!");
    }

    if(0.38<ldr100val&&ldr100val<0.61)
    {
        irradiance = G2(ldr100val);
        Serial.println("no, me!");
    }
    else if (0.32<ldr100val&&ldr100val<0.39)
    {
        irradiance = G3(ldr100val);
        Serial.println("youre mistaken, it was me!");
    }
    else if (0.29<ldr100val&&ldr100val<0.33)
    {
        irradiance = G4(ldr100val);
        Serial.println("pakyu tangina ako yun");
    }

    if (0.37<ldr500val&&ldr500val<0.42)
    {
        irradiance = G5(ldr500val);
        Serial.println("jakol");
    }
    
    Serial.println(irradiance);
    
    delay(1000);
}

//Based on the research paper "Low-Cost Solar Irradiance Meter using LDR Sensors"
float G1(float v)
{
    return 47.52*pow(v,2)-438.01*v+1048;
}

float G2(float v)
{
    return 5512.4*pow(v,2)-6302.7*v+2005.3;
}

float G3(float v)
{
    return 19312*pow(v,2)-16897*v+4049.3;
}

float G4(float v)
{
    return 44630*pow(v,2)-32792*v+6553.5;
}

float G5(float v)
{
    return 47767*pow(v,2)-41939*v+9999.7;
}
