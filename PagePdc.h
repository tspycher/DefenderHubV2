//
// Created by Thomas Spycher on 26.10.22.
//

#ifndef _DEFENDERHUB_PAGEPDC_H
#define _DEFENDERHUB_PAGEPDC_H

#define DISTANCE_FAR 250
#define DISTANCE_CLOSE 10

#include "Page.h"
#include "Colors.h"


class PagePdc : public Page {
public:
  PagePdc(Adafruit_SSD1351 &oled, Defender &car)
    : Page(oled, car){};

  bool needs_display_update();
  void update_display();
  String get_page_name();
private:
  int get_distance_left();
  int get_distance_right();
  unsigned long int last_update;
};

#endif  //_DEFENDERHUB_PAGEPDC_H
