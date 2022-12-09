#include "Page.h"


void Page::draw_centre_string(const char *buf, int x, int y) {
    int16_t x1, y1;
    uint16_t w, h;
    oled.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    oled.setCursor(x - (w / 2), y);
    oled.print(buf);
}

void Page::show_gauge(int x, int y, int width, int height, const char label[], float value, float min, float max) {
    // reset old values
    oled.fillRect(x, y, width, height, 0x0000);

    int r = height/2;

    // draw Gauge Circle + black out lower part again
    int thickness = 1;
    for(int i = 0; i <= thickness; ++i)
      oled.drawCircle(x+width/2, y+height/2, r-i, GREEN);
    oled.fillRect(x, y+r+1, width, r, 0x0000);

    // draw needle
    float needle_angle = 180 / (max - min) * value * 0.0174533; // in radians
    float needle_x = cos(needle_angle)*(float)r*-1.0;
    float needle_y = sin(needle_angle)*(float)r;
    
    oled.drawLine(x+width/2, y+height/2, (x+width/2) + needle_x, (y+height/2) - needle_y, GREEN);

    // set new values
    oled.setTextColor(GREEN);
    oled.setTextSize(1);

    draw_centre_string(String((int)value).c_str(), x+width/2, y+height/2+3);
    draw_centre_string(label, x+width/2, y+height/2+12);
}

void Page::update_label_basics(const char label[], const char value[], int x, int y, int h, bool big_value, int color) {
    int line_space = 2;
    // reset old values
    oled.fillRect(x, y, 128-x, (2*h)+line_space, 0x0000);

    // set new values
    oled.setTextColor(color);
    oled.setTextSize(1);

    oled.setCursor(x,y);
    oled.print(label);

    oled.setCursor(x,y+h+line_space);
    if(big_value)
        oled.setTextSize(2);
    oled.print(value);
}

void Page::update_label(const char label[], const String value, int x, int y, int h, bool big_value, int color) {
    update_label_basics(label, value.c_str(), x, y, h, big_value, color);
}

void Page::update_label(const char label[], const double value, int x, int y, int h, bool big_value, int color) {
    update_label_basics(label, String(value).c_str(), x, y, h, big_value, color);
}

void Page::update_label(const char label[], const int value, int x, int y, int h, bool big_value, int color) {
    update_label_basics(label, String(value).c_str(), x, y, h, big_value, color);
}
