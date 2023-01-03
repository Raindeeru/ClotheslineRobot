int x = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println(exp(x));
    if(x<10)
    {
        x++;
    }
}

//This function measures the evaporation rate using penmann-montieth equation
float EvapRate(float humidity, float temp, float irradiance, float windSpeed)
{
    float e;
    float delta;
    float x = exp(2);
}