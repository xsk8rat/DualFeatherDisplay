//
//
// https://www.sparkfun.com/products/14558

void initRTC() {
  if (rtc.begin() == false) {
    Serial.println("RTC failed to START");
  };
  rtc.set24Hour();
  Serial.println("RTC is cool");
}

void updateRTCFromNTP () {
  time_t currentTime   = getNtpTime();

  Serial.println("Setting the RTC");

  // If the NTP call failed to get the time, then don't set the clock.
  if (currentTime <= 0) {
    Serial.println("Call to NTP is bad.");
    return;
  }
  int    currentYear   = year(currentTime);
  int    currentMonth  = month(currentTime);
  int    currentDay    = day(currentTime);
  int    currentHour   = hour(currentTime);
  int    currentMinute = minute(currentTime);
  int    currentSecond = second(currentTime);
  Serial.print("NTP Time = "); 
  Serial.print(currentYear); Serial.print("-");
  Serial.print(currentMonth); Serial.print("-");
  Serial.print(currentDay); Serial.print(" ");

  Serial.print(currentHour); Serial.print(":");
  Serial.print(currentMinute); Serial.print(":");
  Serial.print(currentSecond); Serial.println("");


  // Fix any wrap around the clock
  if (currentHour < 0)  //handle wraparound
    currentHour = currentHour + 24;
  else
  {
    if (currentHour > 23)
      currentHour = currentHour - 24;
  }
  Serial.println("Setting the RTC");
  if (rtc.setTime(0, currentSecond, currentMinute, currentHour
    , currentDay, currentMonth , currentYear
    ,0 ) == false) {
    Serial.println("RTC failed to SET");
  };
  Serial.println("Done setting the RTC");
}

void WriteTimeToSerial () {
  if (rtc.updateTime() == false) //Updates the time variables from RTC
  {
    Serial.println("RTC failed to update");
    return;
  }
    Serial.println(rtc.stringTime());
}

time_t SYNCRTCFromNTP () {
  time_t currentTime   = getNtpTime();

  Serial.println("SYNCING the RTC from NTP");

  // If the NTP call failed to get the time, then don't set the clock.
  if (currentTime <= 0) {
    Serial.println("Call to NTP is bad.");
    return currentTime;
  }
  int    currentYear   = year(currentTime);
  int    currentMonth  = month(currentTime);
  int    currentDay    = day(currentTime);
  int    currentHour   = hour(currentTime);
  int    currentMinute = minute(currentTime);
  int    currentSecond = second(currentTime);
  Serial.print("NTP Time = "); 
  Serial.print(currentYear); Serial.print("-");
  Serial.print(currentMonth); Serial.print("-");
  Serial.print(currentDay); Serial.print(" ");

  Serial.print(currentHour); Serial.print(":");
  Serial.print(currentMinute); Serial.print(":");
  Serial.print(currentSecond); Serial.println("");


  // Fix any wrap around the clock
  if (currentHour < 0)  //handle wraparound
    currentHour = currentHour + 24;
  else
  {
    if (currentHour > 23)
      currentHour = currentHour - 24;
  }
  if (rtc.setTime(0, currentSecond, currentMinute, currentHour
    , currentDay, currentMonth , currentYear
    ,0
  ) == false) {
    Serial.println("RTC failed to SET");
  };
  Serial.println("Done ness of SYNCING the RTC from NTP");
  return currentTime;
}
