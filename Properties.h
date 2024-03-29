// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include "Secrets.h"

const char SSID[] = SECRET_SSID;           // Network SSID (name)
const char PASS[] = SECRET_OPTIONAL_PASS;  // Network password (use for WPA, or use as key for WEP)

void onAliveChange();

float temperatureInside;
float temperatureOutside;
bool alive;

void initProperties() {

  ArduinoCloud.addProperty(temperatureInside, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(temperatureOutside, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(alive, READWRITE, ON_CHANGE, onAliveChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
