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

    void show_gauge(int x, int y, int width, int height, const char label[], float value, float min, float max);
    
    void update_label(const char label[], const String value, int x, int y, int h, bool big_value=false, int color=0x07E0);
    void update_label(const char label[], double value, int x, int y, int h, bool big_value=false, int color=0x07E0);
    void update_label(const char label[], int value, int x, int y, int h, bool big_value=false, int color=0x07E0);
private:
    void update_label_basics(const char label[], const char value[], int x, int y, int h, bool big_value=false, int color=0x07E0);
    void draw_centre_string(const char *buf, int x, int y);
};

#endif
