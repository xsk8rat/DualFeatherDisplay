void initSegmentDisplay() {
  alpha4.begin(0x71);
  delay(100);
  alpha4.writeDigitAscii(0, 'B');
  alpha4.writeDigitAscii(1, 'O');
  alpha4.writeDigitAscii(2, 'O');
  alpha4.writeDigitAscii(3, 'T');
  alpha4.writeDisplay();
}

void writeFloatToSegment(float inFloat) {
char buf[5];
int useInt = 10*int(inFloat);
snprintf(buf, 4, "%04d", useInt);

for (int i=1; i<3;i++) {
  alpha4.writeDigitAscii(i, buf[i]);
}
alpha4.writeDigitAscii(3, 'F');
alpha4.writeDisplay();
}

void displayTime() {
  if (rtc.updateTime() == false) //Updates the time variables from RTC
  {
    Serial.println("RTC failed to update for display");
    return;
  }
  
    String timeString = rtc.stringTime();
    //alpha4.clear();
    alpha4.writeDigitAscii(0, timeString[0]);
    alpha4.writeDigitAscii(1, timeString[1]);
    alpha4.writeDigitAscii(2, timeString[3]);
    alpha4.writeDigitAscii(3, timeString[4]);
    //alpha4.colonOn();
    alpha4.writeDisplay();
 
}