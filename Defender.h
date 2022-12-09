//
// Created by Thomas Spycher on 02.08.22.
//

#ifndef _DEFENDER_H
#define _DEFENDER_H

#define RX_PIN 0
#define GPS_RX 6
#define GPS_TX 7
#define GPS_BAUD 9600

#define IDENTIFIER_TEMPERATURE 11

#include <Arduino.h>
#include <RCSwitch.h>
#include <TinyGPSPlus.h>

class Defender {
public:
  Defender();
  void begin();

  void update(bool radio = true, bool gps = true, bool obd = true);

  float get_outside_temperature();
  float get_inside_temperature();

  double get_latitude();
  double get_longitude();

  double get_altitude();
  double get_gpsspeed();
  double get_course();
  int get_satellites();

private:
  RCSwitch *receiver;
  TinyGPSPlus *gps;

  void read_433();
  void read_gps();
  void read_obd();
  float outside_temperature;
  float inside_temperature;

  double latitude;
  double longitude;
  double altitude;
  double gpsspeed;
  double course;
  int satellites;
};
#endif //DEFENDERHUB_DEFENDERMENU_H
