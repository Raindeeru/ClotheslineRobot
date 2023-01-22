#include <DHT.h>

unsigned long timemeasure = 2.00; // seconds

float threshold = 5.0; //threshold in mm/day of evap rate

// anemometer parameters
volatile byte rpmcount; // count signals
volatile unsigned long last_micros;
unsigned long timeold;
int timetoSleep = 1;               // minutes
unsigned long sleepTime = 15;      // minutes
unsigned long timeNow;
int countThing = 0;
int GPIO_pulse = 2; // Arduino = D2
float rpm, rps;     // frequencies
float radius = 0.1; // meters - measure of the lenght of each the anemometer wing
float velocity_kmh; // km/h
float velocity_ms;  //m/s
float omega = 0;    // rad/s
float calibration_value = 2.0;

//dht22 parameters
#define DHTpin 10
#define DHTtype DHT22
float temperature;
float humidity;

//Irradiance parameters
int ldr100 = A0;
int ldr500 = A1;
int ldr1k = A2;

float ldr100val;
float ldr500val;
float ldr1kval;

float irradiance;

//Motor A
int enA = 9;
int in1 = 8;
int in2 = 7;

//Motor B
int enB = 3;
int in3 = 5;
int in4 = 4;

bool isOpen = false;

DHT dht(DHTpin, DHTtype);

void setup()
{
    Serial.begin(9600);
    dht.begin();
    
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);

    pinMode(4, INPUT);
    pinMode(13,OUTPUT);

    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);


    pinMode(GPIO_pulse, INPUT_PULLUP);
    digitalWrite(GPIO_pulse, LOW);

    detachInterrupt(digitalPinToInterrupt(GPIO_pulse));                         // force to initiate Interrupt on zero
    attachInterrupt(digitalPinToInterrupt(GPIO_pulse), rpm_anemometer, RISING); //Initialize the intterrupt pin
    rpmcount = 0;
    rpm = 0;
    timeold = 0;
    timeNow = 0;

} // end of setup

void loop()
{
    //irradiance stuff
    ldr100val = analogRead(ldr100)/204.8;
    ldr500val = analogRead(ldr500)/204.8;
    ldr1kval = analogRead(ldr1k)/204.8;

    //I know this code looks ugly, sorry :(
    if(2.75 < ldr1kval && ldr1kval < 4.76)
    {
        irradiance = G1(ldr1kval);
    }

    if(0.38<ldr100val&&ldr100val<0.61)
    {
        irradiance = G2(ldr100val);
    }
    else if (0.32<ldr100val&&ldr100val<0.39)
    {
        irradiance = G3(ldr100val);
    }
    else if (0.29<ldr100val&&ldr100val<0.33)
    {
        irradiance = G4(ldr100val);
    }

    if (0.37<ldr500val&&ldr500val<0.42)
    {
        irradiance = G5(ldr500val);
    }

    //dht stuff
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    if ((millis() - timeold) >= timemeasure * 1000)
    {
    countThing++;

    //wind speeed
    detachInterrupt(digitalPinToInterrupt(GPIO_pulse)); // Disable interrupt when calculating
    rps = float(rpmcount) / float(timemeasure);         // rotations per second
    rpm = 60 * rps;                                     // rotations per minute
    omega = 2 * PI * rps;                               // rad/s
    velocity_ms = omega * radius * calibration_value;   // m/s
    velocity_kmh = velocity_ms * 3.6;                   // km/h
    
    Serial.println("________________________________");

    Serial.print("Wind Speed: ");
    Serial.print(velocity_ms);
    Serial.println(" m/s");

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    Serial.print("Solar Radiation: ");
    Serial.print(irradiance);
    Serial.println("MW/m^2");

    float decHum = humidity * 0.01;
    Serial.println(decHum);
    float evapRate = EvapRate(decHum, temperature, velocity_ms, irradiance);

    Serial.print("Evaporation Rate: ");
    Serial.println(evapRate);

    //checks if irradiance is above the set threshold, you put the motor activation code here
    if(evapRate>threshold && !isOpen)
    {
        digitalWrite(13, 1); //turn on the motor
        
        digitalWrite(in1, 1);
        digitalWrite(in2, 0);        
        digitalWrite(in3, 1);        
        digitalWrite(in4, 0);   
        delay(3000);     

        digitalWrite(in1, 0);
        digitalWrite(in2, 0);        
        digitalWrite(in3, 0);        
        digitalWrite(in4, 0);   
        delay(1000);

    }
    else if (evapRate<threshold && isOpen)
    {
        digitalWrite(13, 0); //turn off the motor

        digitalWrite(in1, 0);
        digitalWrite(in2, 1);        
        digitalWrite(in3, 0);        
        digitalWrite(in4, 1);   
        delay(3000);     

        digitalWrite(in1, 0);
        digitalWrite(in2, 0);        
        digitalWrite(in3, 0);        
        digitalWrite(in4, 0);   
        delay(1000);
    }

    if (countThing == 1) // Send data per timeasure seconds
    {
        countThing = 0;
    }
    timeold = millis();
    rpmcount = 0;
    attachInterrupt(digitalPinToInterrupt(GPIO_pulse), rpm_anemometer, RISING); // enable interrupt
    }

} // end of loop

void rpm_anemometer()
{
  if (long(micros() - last_micros) >= 5000)
  { // time to debounce measures
    rpmcount++;
    last_micros = micros();
  }
}

//This function measures the evaporation
float EvapRate(float h, float t, float u, float r)
{
    //Energy Balance
    float lv = (2501000) - (2370*t);
    float Er = (r)/(lv*997) * 86400000;

    //Aerodynamic
    float B = (0.102*u)/77.53;
    float eas = 611*exp((17.27*t)/(237.3+t));
    float ea = h*eas;
    float Ea = B*(eas - ea);

    //Combined
    float d = (4098*eas)/pow(237.3+t,2);
    float E = ((d/(d+66.8))*Er) + ((66.8/(d+66.8))*Ea);

    return E;
}

//for radiation stuff
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