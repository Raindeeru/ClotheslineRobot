import requests
import math
import keys
import json

SOLCAST_KEY = 'S_zP8SO4EnUbnCQsIVSTooAOo5EMmvNZ'
OPENWEATHERMAP_KEY = '69daeb9d1aaca0b933d930f4e185e3ea'

latitude = '12.9927'
longitude = '124.0147'

solurl = 'https://api.solcast.com.au/world_radiation/estimated_actuals?latitude='+latitude+'&longitude='+longitude+'&hours=48&format=json&api_key='+SOLCAST_KEY

owmreq = requests.get('https://api.openweathermap.org/data/2.5/weather?id=1685759&appid='+ OPENWEATHERMAP_KEY)
owmdata = owmreq.json()

payload={}
headers = {
  'Accept': 'application/json'
}

screq = requests.request("GET", solurl, headers=headers, data=payload)
soldata = screq.json()

temp = owmdata['main']['temp']+273.15
hum = owmdata['main']['humidity']
wind = owmdata['wind']['speed']
rad = soldata['estimated_actuals'][0]['ghi']

print('Temperature = '+str(temp)+'°C')
print('Humidity = '+str(hum)+'%')
print('Wind Speed = '+str(wind)+'m/s')
print('Irradiance = '+str(rad)+'W/m^2')

def EvapEq(h,t,u,r):
    lv = (2501000) - (2370*t)
    print(lv)
    Er = (r)/(lv*997)*86400000
    print(Er)

    b = (0.102*u)/77.53;
    eas = 611*math.exp((17.27*t)/(237.3+t))
    ea = h*eas
    Ea = b*(eas-ea)

    d = (4098*eas)/math.pow(237.3+t,2);
    print(d)
    e = ((d/(d+66.8))*Er)+((66.8/(d+66.8))*Ea)

    return e

evaprate = EvapEq(hum*0.01,temp+273.15,wind,rad)
print('Evaporation Rate = '+str(evaprate)+'mm/day')