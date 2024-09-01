void initOLEDDisplay() {
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default
  display.clearDisplay();
  display.setTextSize(2);
  display.setRotation(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.println(CODENAME);
  display.println(DEVNAME);
  

  display.display();
  delay(1000);
}

void displayWIFIStart () {
  // Display WiFi Notice
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Starting");
  display.println("WiFi");
  display.setTextSize(1);
  display.println(SECRET_SSID);
  display.display();
}

void displayWIFIStatus () {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Connected: ");
  display.setTextSize(1);
  display.print(getIPAddressString());
  display.display();
  delay(2000);
}


// = = = = = = ==============================================================================
//             displayDatas ()
//             Periodically update the display
// = = = = = = ==============================================================================
void displayInfo() {
  long rssi = WiFi.RSSI();
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  int iOffset = random(2);
  display.setCursor(iOffset,0);
  if(temperatureValue<100) {
   display.print(" "); 
  }
  display.print(temperatureValue,1);
  display.println(" F");
  if(humidityValue<100) {
   display.print(" "); 
  }
  display.print(humidityValue,1);
  display.println(" %RH");
  // display.print(pressureValue,1);
  // display.println(" hPa");
  // display.print(gasValue,1);
  // display.println(" kOhm");

  display.setTextSize(0);
  display.println(DEVNAME);
  display.println(CODENAME);
  display.print(getIPAddressString());
  display.println();
  display.print(SECRET_SSID);
  display.print("   ");
  display.print(int(rssi));
  display.print("dB");
  delay(10);
  yield();
  display.display();
}
