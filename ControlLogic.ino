/*************************************************************
  Here we do the control logic.
  Check all operation modes.
  Compare setpoint setting and temperature measurment
 *************************************************************/

void OperationLogic() {

  if (workMode) { // Check for working mode true = automatic, false = manual
    bool timerOne = false;
    bool timerTwo = false;

    if (automaticMode) { // Check for automatic mode true = time input + temp, false = temp only

      timerOne = isTimerOne;
      timerTwo = isTimerTwo;

      isWork = SetpointCheck(timerOne, timerTwo);
    }
    else {
      timerOne = true;
      isWork = SetpointCheck(timerOne, timerTwo);
    }
  }
  else { // Manual mode we send dirrect false = stopped, true = working
    isWork = manualMode;
  }

  if (lastState != isWork) { // Check and only if isWork command is changed we send it
    Device1.virtualWrite(V100, isWork); // Send state to control device.
    
    if (isWork) {
      led1.on();
    }
    else {
      led1.off();
    }
    
    lastState = isWork;
  }
}

bool SetpointCheck(bool timerOne, bool timerTwo) {

  bool thermostat;

  if (timerOne) {
    if (temperatureSensor > setpoint + 1) {
      timerOne = false;
    }
    else if (temperatureSensor <= setpoint - 1) {
      timerOne = true;
    }
  }

  if (timerTwo) {
    if (temperatureSensor > setpoint2 + 1) {
      timerTwo = false;
    }
    else if (temperatureSensor <= setpoint2 - 1) {
      timerTwo = true;
    }
  }

  if (timerOne || timerTwo) {
    thermostat = true;
  }
  else {
    thermostat = false;
  }

  return thermostat;
}
