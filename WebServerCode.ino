// = = = = = = ==============================================================================
//             serveWebPage()
//             Provide smoothed data from the global variables
// = = = = = = ==============================================================================
void serveWebPage() {
    WiFiClient client = server.available();
    float usedBm  = float(WiFi.RSSI());
  if (client) {
    //Serial.println("Serving data web page.");
  IPAddress ip;
  ip = WiFi.localIP();

    // an http request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          //send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 60");  // refresh the page automatically every 5 sec
          client.println();
          // client.println("<!DOCTYPE HTML>");
          // client.println("<html>");
          //output the value of each parameter
          dataObject["temperaturevalue"]  = temperatureValue;
          dataObject["temperatureunits"]  = "F";
          dataObject["temperaturesensor"] = "BME680";
          
          dataObject["humidityvalue"]  = humidityValue;
          dataObject["humidityunits"]  = "%R";
          dataObject["humiditysensor"] = "BME680";
          
          dataObject["pressurevalue"]  = pressureValue;
          dataObject["pressureunits"]  = "hPa";
          dataObject["pressuresensor"] = "BME680";
            
          dataObject["gasTVOCvalue"]  = gasValue;
          dataObject["gasTVOCunits"]  = "kOhms";
          dataObject["gasTVOCsensor"] = "BME680";


          dataObject["wifipowervalue"]  = usedBm;
          dataObject["wifipowerunits"]  = "dBmW";
          dataObject["wifipowersensor"] = "None";
          dataObject["wifiIPNumber"] = IpAddress2String(ip);

          dataObject["CODEDIR"]  = CODEDIR;
          dataObject["CODENAME"] = CODENAME;
          dataObject["DEVNAME"]  = DEVNAME;
          dataObject["location"] = LOCATION;

          client.print(dataObject);
          // client.print(pressureValue);
          // client.print(" ");
          // client.print((humidityValue));
          // client.print(" ");
          // client.print(temperatureValue);
          // client.print(" ");
          // client.print(CODEDIR);
          // client.println(CODENAME);
          //client.println("</br>");
          // client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    //Serial.println("Done serving web data.");
  }
}

// = = = = = = ==============================================================================
//             restartWebserverThingy()
//             Restart the WiFi and webserver if not connected.
// = = = = = = ==============================================================================

void restartWebserverThingy() {
    // If WiFi and/or server are NOT working
  if (WiFi.status() != WL_CONNECTED) {
     Serial.println("WiFi Disconnected!");
     Serial.print("Wifi Status = ");
     Serial.println(status);
     //printWifiStatus();
     server.flush();
     delay(2000);

    status = WiFi.begin(ssid, pass);
    while ( status != WL_CONNECTED) {
      Serial.println("Reconnecting...");
      status = WiFi.begin(ssid, pass);
      // wait 10 seconds for connection:
      wdt_disable( );
      delay(10000);
      wdt_reEnable( );
    }
    Serial.println("WiFi Reconnected");
    printWifiStatus();
    server.begin();
  }
}

void startWiFiSystem () {
  displayWIFIStart ();
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Connecting to SSID     -> ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  printWifiStatus();

}


// = = = = = = ==============================================================================
//             IpAddress2String ()
//             https://forum.arduino.cc/t/how-to-manipulate-ipaddress-variables-convert-to-string/222693/5
// = = = = = = ==============================================================================
String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}

// = = = = = = ==============================================================================
//             vPrintWiFiStatusString ()
//             Prints string which describes current wifi state.
// = = = = = = ==============================================================================
void vPrintWiFiStatusString() {
  int iWiFi = WiFi.status();
  if (iWiFi == WL_CONNECTED)      {Serial.print("WL_CONNECTED");      return;};
  if (iWiFi == WL_NO_SHIELD)      {Serial.print("WL_NO_SHIELD");      return;};
  if (iWiFi == WL_CONNECT_FAILED) {Serial.print("WL_CONNECT_FAILED"); return;};
  if (iWiFi == WL_NO_SSID_AVAIL)  {Serial.print("WL_NO_SSID_AVAIL");  return;};
  if (iWiFi == WL_SCAN_COMPLETED) {Serial.print("WL_SCAN_COMPLETED"); return;};
  if (iWiFi == WL_DISCONNECTED)   {Serial.print("WL_DISCONNECTED");   return;};
  Serial.print("WiFi Code=");
  Serial.print(iWiFi);

}


// = = = = = = ==============================================================================
//              getIPAddressString ()
//              Abstracting, as it may be different for different systems.
// = = = = = = ==============================================================================
String getIPAddressString() {
  IPAddress ip = WiFi.localIP();
  return IpAddress2String(ip);
}
