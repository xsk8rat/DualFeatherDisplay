Displays the time averaged sensor values and network info to the OLED.

Uses the bigger segmented display for a clock, which is set using an RTC and periodic NTP updates.
![IMG_1399 - Copy](https://github.com/user-attachments/assets/9d05a561-1176-433b-8bcf-823b6cab56e3)

Serves a webpage with the sensor data in JSON format: 
  {"temperaturevalue":77.984588623046875,"temperatureunits":"F","temperaturesensor":"BME680","humidityvalue":41.939704895019531,"humidityunits":"%R","humiditysensor":"BME680","pressurevalue":991.0701904296875,"pressureunits":"hPa","pressuresensor":"BME680","gasTVOCvalue":161.90249633789062,"gasTVOCunits":"kOhms","gasTVOCsensor":"BME680","wifipowervalue":-41,"wifipowerunits":"dBmW","wifipowersensor":"None","wifiIPNumber":"192.168.xx.xx","CODEDIR":"WebHumidity","CODENAME":"OLED_14Seg_BME680_006","DEVNAME":"WebData_013","location":"Indeterminate"}

  There are some RaspberryPi scripts that can scrape the sensor periodically in the repo: https://github.com/xsk8rat/Basic-WebSensor-M0-WIN1500


The code runs on "Adafruit Feather M0 WiFi": https://www.adafruit.com/product/3044

OLED: https://www.adafruit.com/product/4650

Segmented LED: https://www.adafruit.com/product/3128

Doubler: https://www.adafruit.com/product/2890

Sensor: https://www.adafruit.com/product/3660

RTC: https://www.sparkfun.com/products/14558 (Since it sets the clock with NTP, it doesn't 100% _need_ the battery. But it's what i have on hand.)

Case for 3D printing is located at: 
https://www.thingiverse.com/thing:6751555

