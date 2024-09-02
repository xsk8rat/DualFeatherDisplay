Serves a webpage with the sensor data in JSON format: 
  {"temperaturevalue":77.542495727539062,"temperatureunits":"F","temperaturesensor":"BME680","humidityvalue":40.070812225341797,"humidityunits":"%R","humiditysensor":"BME680","pressurevalue":990.70745849609375,"pressureunits":"hPa","pressuresensor":"BME680","wifipowervalue":-42,"wifipowerunits":"dBmW","wifipowersensor":"None","wifiIPNumber":"192.168.XX.XX","CODEDIR":"WebHumidity","CODENAME":"OLED_14Seg_BME680_006","DEVNAME":"WebData_013","location":"Indeterminate"}

Displays the sensor values and network info to the OLED.

Uses the bigger segmented display for a clock, which is set using an RTC and periodic NTP updates.


Case is located at:

https://www.thingiverse.com/thing:6751555

The code runs on "Adafruit Feather M0 WiFi": https://www.adafruit.com/product/3044

OLED: https://www.adafruit.com/product/4650
Segmented LED: https://www.adafruit.com/product/3128
Doubler: https://www.adafruit.com/product/2890
Sensor: https://www.adafruit.com/product/3660
RTC: https://www.sparkfun.com/products/14558
