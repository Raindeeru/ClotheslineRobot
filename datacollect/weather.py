import requests
import math
import keys
import json

SOLCAST_KEY = keys.SOLCAST_KEY 
OPENWEATHERMAP_KEY = keys.OPENWEATHERMAP_KEY

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

print(json.dumps(owmdata, indent=4))

temp = owmdata['main']['temp']+273.15
hum = owmdata['main']['humidity']
wind = owmdata['wind']['speed']
rad = soldata['estimated_actuals'][0]['ghi']

print('Temperature = '+str(temp)+'°C')
print('Humidity = '+str(hum)+'%')
print('Wind Speed = '+str(wind)+'m/s')
print('Irradiance = '+str(rad)+'W/m^2')

