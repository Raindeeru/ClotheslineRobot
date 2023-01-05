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
#define DHTpin 4
#define DHTtype DHT22
float temperature;
float humidity;

//Irradiance standin
float irradiance;

DHT dht(DHTpin, DHTtype);

void setup()
{
    dht.begin();
    
    pinMode(A0, INPUT);
    pinMode(4, INPUT);
    pinMode(13,OUTPUT);

    pinMode(GPIO_pulse, INPUT_PULLUP);
    digitalWrite(GPIO_pulse, LOW);

    Serial.begin(9600);

    detachInterrupt(digitalPinToInterrupt(GPIO_pulse));                         // force to initiate Interrupt on zero
    attachInterrupt(digitalPinToInterrupt(GPIO_pulse), rpm_anemometer, RISING); //Initialize the intterrupt pin
    rpmcount = 0;
    rpm = 0;
    timeold = 0;
    timeNow = 0;

} // end of setup

void loop()
{
    irradiance = analogRead(A0)/100;
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    //Measure RPM
    if ((millis() - timeold) >= timemeasure * 1000)
    {
    countThing++;
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

    if(evapRate>threshold)
    {
        digitalWrite(13, 1);
        Serial.println("yo yo yo!");
    }
    else
    {
        digitalWrite(13, 0);
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
  //   Serial.println("***** detect *****");
}

//This function measures the evaporation
float EvapRate(float h, float t, float u, float r)
{
    //Energy Balance
    float lv = (2501000) - (2370*t);
    float Er = (r*41.66)/(lv*997) * 86400000;

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