
void GetTemperature() {
  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)

  //delay(dht.getMinimumSamplingPeriod());
  rawHumidity += dht.getHumidity(); // Sum all readings
  rawTemperature += dht.getTemperature(); // Sum all readings

  if (readCount == 9) {
    humiditySensor = (rawHumidity /= 10); // Divide readings to counter for average result
    temperatureSensor =  (rawTemperature /= 10) - 2;

    Blynk.virtualWrite(V0, (int)temperatureSensor); // Send widget temp. to update it.
    Blynk.virtualWrite(V1, (int)humiditySensor); // Send widget humidity to update it.

    readCount = 0;
  }
  else {
    readCount++;
  }

  if (isnan(humiditySensor)) {
    Serial.println("Failed to read humidity sensor!");
    //return;
  }
  if (isnan(temperatureSensor)) {
    Serial.println("Failed to read temperature sensor!");
    return;
  }
}

BLYNK_WRITE(V4) { // Button Widget function

  workMode = param.asInt();

  if (param.asInt() == 1) {
    Blynk.virtualWrite(V8, 0);
    manualMode = false;
  }
}

BLYNK_WRITE(V5) { // Button Widget function

  switch (param.asInt()) {
    case 1:
      automaticMode = true;
      break;

    case 2:
      automaticMode = false;
      break;
  }
}

BLYNK_WRITE(V7) { // Button Widget function

  setpoint = param.asInt();
  Blynk.virtualWrite(V6, setpoint); // Send widget setpoint to update it.
}

BLYNK_WRITE(V8) { // Button Widget function

  if (!workMode &&  param.asInt() == 1) {
    Blynk.virtualWrite(V8, 1);
    manualMode = true;
  }
  else {
    Blynk.virtualWrite(V8, 0);
    manualMode = false;
  }
}

void activetoday() {        // check if schedule #1 should run today
  if (year() != 1970) {
    Blynk.syncVirtual(V9);  // sync scheduler #1
  }
}

void clockDisplay() { // only needs to be done once after time sync
  if ((year() != 1970) && (clockSync == false)) {
    sprintf(currentTime, "%02d:%02d:%02d", hour(), minute(), second());
    Serial.println(currentTime);
    clockSync = true;
  }
}

BLYNK_WRITE(V9) {   // Scheduler #1 Time Input widget
  TimeInputParam t(param);
  unsigned int nowseconds = ((hour() * 3600) + (minute() * 60) + second());
  unsigned int startseconds = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);
  unsigned int stopseconds = (t.getStopHour() * 3600) + (t.getStopMinute() * 60);
  int dayadjustment = -1;

  if (weekday() == 1) {
    dayadjustment = 6; // needed for Sunday Time library is day 1 and Blynk is day 7
  }
  if (t.isWeekdaySelected((weekday() + dayadjustment))) { //Time library starts week on Sunday, Blynk on Monday
    //Schedule is ACTIVE today
    if (!isTimerOne && nowseconds >= startseconds) { // - 31 && nowseconds <= startseconds + 31 ) {  // 62s on 60s timer ensures 1 trigger command is sent
      isTimerOne = true;  // turn on virtual LED
      //Serial.println("Schedule 1 started");
    }
    if (isTimerOne && nowseconds >= stopseconds) { // - 31 && nowseconds <= stopseconds + 31 ) { // 62s on 60s timer ensures 1 trigger command is sent
      isTimerOne = false;   // turn OFF virtual LED
      //Serial.println("Schedule 1 finished");
    }
  }
}

BLYNK_WRITE(V10) { // Button Widget function

  additionalTimer = param.asInt();
  //Serial.println(additionalTimer);
  if (!additionalTimer) {
    isTimerTwo = false;
  }
}

void activetoday2() {        // check if schedule #1 should run today
  if (year() != 1970) {
    Blynk.syncVirtual(V11);  // sync scheduler #1
  }
}

BLYNK_WRITE(V11) {   // Scheduler #2 Time Input widget
  if (additionalTimer) {
    TimeInputParam t(param);
    unsigned int nowseconds = ((hour() * 3600) + (minute() * 60) + second());
    unsigned int startseconds = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);
    unsigned int stopseconds = (t.getStopHour() * 3600) + (t.getStopMinute() * 60);
    int dayadjustment = -1;
    
    if (weekday() == 1) {
      dayadjustment = 6; // needed for Sunday Time library is day 1 and Blynk is day 7
    }
    if (t.isWeekdaySelected((weekday() + dayadjustment))) { //Time library starts week on Sunday, Blynk on Monday
      //Schedule is ACTIVE today
      if (!isTimerTwo && nowseconds >= startseconds) {
        isTimerTwo = true;  // turn on virtual LED
        //Serial.println("Schedule 2 started");
      }
      if (isTimerTwo && nowseconds >= stopseconds) {
        isTimerTwo = false;   // turn OFF virtual LED
        //Serial.println("Schedule 2 finished");
      }
    }
  }
  else {

    isTimerTwo = false;
  }
}

BLYNK_WRITE(V13) { // Button Widget function

  setpoint2 = param.asInt();
  Blynk.virtualWrite(V12, setpoint2); // Send widget setpoint to update it.

}
