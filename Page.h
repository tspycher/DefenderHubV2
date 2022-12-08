//
// Created by Thomas Spycher on 02.08.22.
//

#ifndef _DEFENDERHUB_PAGE_H
#define _DEFENDERHUB_PAGE_H

#include "Colors.h"
#include <Arduino.h>
#include <Adafruit_SSD1351.h>
#include "Defender.h"


class Page {
public:
    Page(Adafruit_SSD1351 &oled, Defender &car) : oled(oled), car(car) {};
    virtual bool needs_display_update() { return false; }
    virtual void update_display() {}
    virtual String get_page_name() {}

protected:
    Adafruit_SSD1351& oled;
    Defender& car;
};

#endif
