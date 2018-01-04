# Ethernet-LCD-DHT-Sensor
Arduino Uno project that utilizes DH22 sensor for sensing temperature and humidity, Ethernet Shield for connecting the ethernet, a buzzer that beeps periodically when set values are exceeded, triggers a relay when a set humidity or temperature is exceeded (I turn on the A/C with the relay) and responds to JSON requests (temperature, humidity and A/C on or off)

Links to extrenal libraries used:
https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
https://github.com/Wiznet/WIZ_Ethernet_Library
https://arduinojson.org/
https://github.com/adafruit/DHT-sensor-library
https://github.com/adafruit/Adafruit_Sensor

I have another version which keeps the relay on while the limits are exceeded. I can upload it as well if requested.
