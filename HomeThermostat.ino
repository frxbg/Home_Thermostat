/*************************************************************
  This is a DEMO. You can use it only for development and testing.
  You should open Setting.h and modify General options.

  If you would like to add these features to your product,
  please contact Blynk for Businesses:

                   http://www.blynk.io/

 *************************************************************/

//#define USE_SPARKFUN_BLYNK_BOARD    // Uncomment the board you are using
#define USE_NODE_MCU_BOARD        // Comment out the boards you are not using
//#define USE_WITTY_CLOUD_BOARD
//#define USE_CUSTOM_BOARD          // For all other ESP8266-based boards -
// see "Custom board configuration" in Settings.h

#define APP_DEBUG        // Comment this out to disable debug prints

#define BLYNK_PRINT Serial

#include "BlynkProvisioning.h"

void setup() {
  delay(500);
  Serial.begin(115200);

  BlynkProvisioning.begin();
  Thermostat_init(); // Initialize this example
}

void loop() {
  // This handles the network and cloud connection
  BlynkProvisioning.run();
  Thermostat_run();
}


/*************************************************************
  Home thermostat with week timer and DHT11
 *************************************************************/

/* Comment this out to disable prints and save space */
#include "DHTesp.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

#include <TimeLib.h>
#include <WidgetRTC.h>

DHTesp dht;
WidgetLED led1(V101);
WidgetRTC rtc;
WidgetBridge Device1(V100);
BlynkTimer timer; // Announcing the timer

float rawHumidity = 0;
float rawTemperature = 0;  // Values read from sensor

float temperatureSensor = 0;
float humiditySensor = 0;

char currentTime[9];
byte setpoint = 0; // Setpoint temp.
byte setpoint2 = 0; // Second setpoint function.
bool workMode = true; // Variable for working mode true = automatic, false = manual
bool automaticMode = true; // Variable for automatic mode true = time input + temp, false = temp only
bool manualMode = false; // Variable for manual mode false = stopped, true = working
bool isWork = false; // Is output is working
bool isTimerOne = true; // Varible for timer 1 true = start time false = stop time
bool additionalTimer = false; // Is additional timer and second setpoint activated
bool isTimerTwo = false; // Varible for additional timer true = start time false = stop time
bool lastState = false; // Varible for sure command to other device is sended only when control state is changed
bool clockSync = false;
byte readCount = 0; // Counter for DHT readings

BLYNK_CONNECTED() {
  rtc.begin();
  Device1.setAuthToken("SeccondDeviceToken");
}

void Thermostat_init() {

  timer.setInterval(1000, GetTemperature); //timer will run every 10 sec.
  timer.setInterval(60000L, activetoday);  // check every 60s if ON / OFF trigger time has been reached
  timer.setInterval(1000L, clockDisplay);  // check every second if time has been obtained from the server
  timer.setInterval(5000L, OperationLogic);  // every 5 secconds go to OperationLogic

  if (additionalTimer) {
    timer.setInterval(60000L, activetoday2);  // check every 60s if ON / OFF trigger time has been reached
  }

  dht.setup(2, DHTesp::DHT11); // Connect DHT sensor to GPIO 2 = D4
  
}

void Thermostat_run() {

  timer.run();        // run timer
}
