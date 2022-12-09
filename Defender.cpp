#include "defender.h"

Defender::Defender() : outside_temperature(0.0), inside_temperature(0.0), latitude(0.0), longitude(0.0), altitude(0.0), gpsspeed(0.0), course(0.0), satellites(999) {

}

void Defender::begin() {
  Serial.println("*** initalizing Defender Class");
  receiver = new RCSwitch();
  receiver->enableReceive(RX_PIN);  // Receiver on interrupt 0 => that is pin #2
  Serial.println("***** initalized 433mhz receiver");


  gps = new TinyGPSPlus();
  Serial1.begin(9600);
  Serial.println("***** initalized gps classes");
}

void Defender::update(bool radio, bool gps, bool obd) {
  if(radio)
    read_433();
  
  if(gps)
    read_gps();
  
  if(obd)
    read_obd();
}

float Defender::get_inside_temperature() {
  return inside_temperature;
}

float Defender::get_outside_temperature() {
  //return (float)random(-10, 39);
  return outside_temperature;
}

double Defender::get_latitude() {
  return latitude;
}

double Defender::get_longitude() {
  return longitude;
}

double Defender::get_altitude() {
  return altitude;
}

double Defender::get_gpsspeed() {
  return gpsspeed;
}

double Defender::get_course() {
  return course;
}

int Defender::get_satellites() {
  return satellites;
}

void Defender::read_gps() {
  while (Serial1.available() > 0) {
    gps->encode(Serial1.read());
  }

  if (gps->satellites.isValid()) {
    satellites = gps->satellites.value();
  }

  if (gps->location.isValid()) {
    latitude = gps->location.lat();
    longitude = gps->location.lng();
  }

  if (gps->altitude.isValid()) {
    altitude = gps->altitude.value();
  }

  if (gps->course.isValid()) {
    course = gps->course.deg();
  }

  if (gps->speed.isValid()) {
    gpsspeed = gps->speed.kmph();
  }
}

void Defender::read_obd() {
  
}

void Defender::read_433() {
  if (receiver->available()) {
    int bitlength = receiver->getReceivedBitlength();
    unsigned long value = receiver->getReceivedValue();
    int identifier = (value / 100000000);
    receiver->resetAvailable();

    switch(identifier) {
      case IDENTIFIER_TEMPERATURE:
        inside_temperature = (value % 100000000 / 1000000.0) - 20.0;
        outside_temperature = (value % 10000 / 100.0) - 20.0;
        Serial.print("433: Received Temperature Inside: ");
        Serial.print(inside_temperature);
        Serial.print(" Outside: ");
        Serial.println(outside_temperature);
        break;
      default:
        Serial.print("433: unknown package received. Value is:");
        Serial.print(value);
        Serial.print(" and identifier is: ");
        Serial.println(identifier);
        break;
    }
  }
}