//
// Created by Thomas Spycher on 26.10.22.
//

#ifndef _DEFENDERHUB_COMPASS_H
#define _DEFENDERHUB_COMPASS_H

#include "Page.h"

class PageCompass : public Page {
public:
    PageCompass(Adafruit_SSD1351 &oled, Defender &car) : Page(oled, car) {};

    bool needs_display_update();
    void update_display();
    String get_page_name();
};

#endif //_DEFENDERHUB_COMPASS_H
