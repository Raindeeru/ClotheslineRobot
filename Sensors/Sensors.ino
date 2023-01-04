#include <DHT.h>

unsigned long timemeasure = 1.00; // seconds

// anemometer parameters
volatile byte rpmcount; // count signals
volatile unsigned long last_micros;
unsigned long timeold;
int timetoSleep = 1;               // minutes
unsigned long sleepTime = 15;      // minutes
unsigned long timeNow;
int countThing = 0;
int GPIO_pulse = 4; // Arduino = D2
float rpm, rps;     // frequencies
float radius = 0.1; // meters - measure of the lenght of each the anemometer wing
float velocity_kmh; // km/h
float velocity_ms;  //m/s
float omega = 0;    // rad/s
float calibration_value = 2.0;

//dht22 parameters
#define DHTpin 2
#define DHTtype DHT22
float temperature;
float humidity;

//Irradiance standin
float irradiance;

DHT dht(DHTpin, DHTtype);

void setup()
{
    dht.begin();
    
    pinMode(4, INPUT);
    pinMode(A0, INPUT);

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
    irradiance = analogRead(A0);
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
    Serial.print(irradiance/50);
    Serial.println("MW/m^2");

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