//
// Created by Thomas Spycher on 26.10.22.
//

#ifndef _DEFENDERHUB_TEMPERATURE_H
#define _DEFENDERHUB_TEMPERATURE_H

#include "Page.h"
#include "Logos.h"


class PageTemperature : public Page {
public:
    PageTemperature(Adafruit_SSD1351 &oled, Defender &car) : Page(oled, car) {};

    bool needs_display_update();
    void update_display();
    String get_page_name();

private:
  float inside_temperature;
  float outside_temperature;

  float inside_humidity;
  float outside_humidity;
};

#endif //DEFENDERHUB_TEMPERATURE_H
