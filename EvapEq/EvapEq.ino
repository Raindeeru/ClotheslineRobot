void setup()
{
    Serial.begin(9600);
    float test = EvapRate(0.87, 286, 10.08, 0);
    Serial.println(test);
}

void loop()
{

}

//This function measures the evaporation rate using penmann-montieth equation
float EvapRate(float humidity, float temp, float irradiance, float windSpeed)
{
    float evapRate;
    float lambda(2.501 - 0.002361*(temp - 273));
    float y = 165.02/lambda;
    float ea = exp(21.07 - (5336/temp));
    float D = (1-humidity)*ea;
    float delta = (5336/pow(temp,2))*D;

    evapRate = (delta*irradiance + y*(6.43*(1+0.536*windSpeed)*D))/(lambda*(delta+y));
    return evapRate;
}