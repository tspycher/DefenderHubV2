#include "Page.h"

void Page::update_label_basics(Adafruit_SSD1351 &oled, const char label[], int x, int y, int h, bool big_value, int color) {
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
}
void Page::update_label(Adafruit_SSD1351 &oled, const char label[], const String value, int x, int y, int h, bool big_value, int color) {
    update_label_basics(oled, label, x, y, h, big_value, color);
    oled.print(value);
}

void Page::update_label(Adafruit_SSD1351 &oled, const char label[], const double value, int x, int y, int h, bool big_value, int color) {
    update_label_basics(oled, label, x, y, h, big_value, color);
    oled.print(value);
}

void Page::update_label(Adafruit_SSD1351 &oled, const char label[], const int value, int x, int y, int h, bool big_value, int color) {
    update_label_basics(oled, label, x, y, h, big_value, color);
    oled.print(value);}
