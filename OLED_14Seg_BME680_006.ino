/*
 * https://learn.adafruit.com/adafruit-gfx-graphics-library
 * 
 * https://www.sparkfun.com/products/16467
 *  0x70  

 * 000 - Basic data webserver with no display configured for the BME280
 * 001 - Added JSON
 * 003 - Changed to forced mode of sampling the sensor.
 * 006 - Added the RTC, ALPHA, and NTP
*/
#include <SPI.h>
#include <Wire.h>
#include <wdt_samd21.h>
char CODENAME[] = "OLED_14Seg_BME680_006";
char CODEDIR[]  = "WebHumidity";
char DEVNAME[]  = "WebData_013";
char LOCATION[]  = "Indeterminate";

// Display
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);


//Sensor(s)
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
Adafruit_BME680 bme; // I2C


// Wifi Stuff https://www.arduino.cc/reference/en/libraries/wifi101/

#include <WiFi101.h>

#include "arduino_secrets.h" 
char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
//WiFiClient client;
WiFiServer server(80);

#include <SoftTimers.h>
SoftTimer sampleTimer;
SoftTimer serialTimer;
SoftTimer display_timer;
SoftTimer ntp_timer;

// = = = = = = 
// Runnning averages - will glitch a bit on re-start!
#include <Ewma.h>
Ewma  pressureFilter(0.005);
Ewma  humidityFilter(0.1);
Ewma  temperatureFilter(0.1);
float pressureValue       = 0;
float humidityValue       = 0;
float temperatureValue    = 0;

Ewma gasFilter(0.05);        float gasValue = 0; 

bool isFirstPass = true;

#include <Arduino_JSON.h>
JSONVar dataObject;

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

// NTP Functionality
#include <WiFiUdp.h>
WiFiUDP Udp;
unsigned int localPort = 2390;
IPAddress timeServerIP;
const char* ntpServerName = "us.pool.ntp.org";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];


// Time and Time Zone Functionality
#include <TimeLib.h>  // https://github.com/PaulStoffregen/Time
time_t UpDateSynchInterval = 24*60; // In seconds -> Every 24 hours
const int timeZone = -5;
#include <Time.h>
// RTYC Functionality
#include <SparkFun_RV1805.h>
RV1805 rtc;

// Time Zone
#include <Timezone.h>
TimeChangeRule myDST = {"CDT", Second, Sun, Mar, 2, 60*(timeZone)};
TimeChangeRule mySTD = {"CST", First,  Sun, Nov, 2, 60*(timeZone-1)};
Timezone myTZ(myDST, mySTD);
TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev
time_t utc, local;




// = = = = = = ==============================================================================
//             setup()
// = = = = = = ==============================================================================
void setup() {

  Serial.begin(115200);
  delay(3000);
  Serial.println("");
  Serial.println("");
  Serial.print("Starting Code:         -> ");
  Serial.print(CODEDIR);
  Serial.println(CODENAME);


  //Initialize the display
  initOLEDDisplay(); 
  
  
  // Start Sensors
  
  Wire.begin();

  // Start the 14 Segment LED
  initSegmentDisplay();

  initializeBME680();

  // Start WiFi and Server Initialization
  startWiFiSystem();
  server.begin();

  displayWIFIStatus();
  delay(2000);

  // Initialize the timers
  timerResetSeconds(sampleTimer,    2.0);
  timerResetSeconds(serialTimer,   10.0);
  timerResetSeconds(ntp_timer,     60.0*24.0);
  timerResetSeconds(display_timer, 10.0);

  // Set up watch dog timer if something goes wrong on the device.
  wdt_init ( WDT_CONFIG_PER_16K );

  // RTC Initialization
  initRTC();
  delay(1000);

  // Start the NTP stuffs
  initalizeNTP(localPort);
  WriteTimeToSerial();

  Serial.print("Running Code           -> ");
  Serial.print(CODEDIR);
  Serial.println(CODENAME);
  Serial.println("Init. and Setup        -> Complete!");
}

// = = = = = = ==============================================================================
//              loop()
// = = = = = = ==============================================================================
void loop() {
  // This will prevent the watchdog from firing.
  wdt_reset ( );
  
  // update running mean  (periodically)
  if (sampleTimer.hasTimedOut() | isFirstPass) {
    sampleSensorData();
    sampleTimer.reset();
  } 
  // Print to Serial line (periodically)
  if (serialTimer.hasTimedOut() | isFirstPass) {
    printInfo();
    serialTimer.reset();
  } 
  // Update the RYC (periodically)
  if (ntp_timer.hasTimedOut()) {
    updateRTCFromNTP();
    ntp_timer.reset();
  } 

  //Update the display
  if (display_timer.hasTimedOut() || isFirstPass) {
    displayInfo();
    //writeFloatToSegment(temperatureValue);
    displayTime();
    display_timer.reset();
    isFirstPass = false;
  } 
  isFirstPass = false;

  restartWebserverThingy();
  serveWebPage();
  delay(500);
}


// = = = = = = ==============================================================================
//             printInfo()
// Prints smoothed (when available) values to the serial port.
void printInfo() {
  float usedBm  = float(WiFi.RSSI());
  Serial.print(temperatureValue); Serial.print(" ");
  Serial.print(humidityValue);    Serial.print(" ");
  Serial.print(pressureValue);    Serial.print(" ");
  Serial.print(gasValue);         Serial.print(" ");
  Serial.print(usedBm);           Serial.print(" ");
  vPrintWiFiStatusString();       Serial.print(" ");

  Serial.print(CODEDIR);
  Serial.print(CODENAME);
  Serial.print("/");
  Serial.print(DEVNAME);
  Serial.print("/");
  Serial.print(LOCATION);
  Serial.println();
}

// = = = = = = ==============================================================================
//             printWifiStatus()
// Prints WiFi info to Serial Port
void printWifiStatus() {
  // print the SSID of the network to which you're attached:
  Serial.print("Connected to SSID      -> ");
  Serial.println(WiFi.SSID());
  // Print the IP address of the device
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address             -> ");
  Serial.println(ip);
  byte mac[6]; 
  WiFi.macAddress(mac);
  Serial.print("MAC Address            -> ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.print(mac[0],HEX);
  Serial.println();
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal Strength (RSSI) -> ");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("FirmWare Version:      -> ");
  Serial.print(WiFi.firmwareVersion());
  Serial.println("");
}

// = = = = = = ==============================================================================
//             timerReset
// Sets the timers usingf the same code,
void timerResetSeconds(SoftTimer &inTimer, float inSec) {
  inTimer.setTimeOutTime(1);
  inTimer.reset();
  delay(2);
  inTimer.setTimeOutTime(inSec*1000); //Resample for time average 
  inTimer.reset();
}
