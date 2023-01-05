void setup()
{
    Serial.begin(9600);
    float test = EvapRate(0.5, 13, 2, 4);
    Serial.println(test);
}

void loop()
{

}

//This function measures the evaporation
float EvapRate(float h, float t, float u, float r)
{
    //Energy Balance
    float lv = (2501000) - (2370*t);
    float Er = (r*41.66)/(lv*997) * 86400000;
    Serial.println(Er);

    //Aerodynamic
    float B = (0.102*u)/77.53;
    float eas = 611*exp((17.27*t)/(237.3+t));
    float ea = h*eas;
    float Ea = B*(eas - ea);
    Serial.println(Ea);

    //Combined
    float d = (4098*eas)/pow(237.3+t,2);
    float E = ((d/(d+66.8))*Er) + ((66.8/(d+66.8))*Ea);

    return E;
}