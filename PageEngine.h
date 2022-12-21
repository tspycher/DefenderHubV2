//
// Created by Thomas Spycher on 26.10.22.
//

#ifndef _DEFENDERHUB_ENGINE_H
#define _DEFENDERHUB_ENGINE_H

#include "Page.h"

class PageEngine : public Page {
public:
  PageEngine(Adafruit_SSD1351 &oled, Defender &car)
    : last_update(0), Page(oled, car){};

  bool needs_display_update();
  void update_display();
  String get_page_name();
private:
  unsigned long int last_update;

};

#endif  //_DEFENDERHUB_ENGINE_H
