//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageEngine.h"

String PageEngine::get_page_name() {
  return String("Engine");
}


bool PageEngine::needs_display_update() {
  return true;
}


void PageEngine::update_display() {
  int margin_top = 20;
  int h = 50;
  show_gauge(0,margin_top, oled.width()/2, h-5, "OilT", (int)random(0,180), 0, 180);
  show_gauge(0,margin_top+h, oled.width()/2, h-5, "OilP", (int)random(0,180), 0, 180);

  show_gauge(oled.width()/2,margin_top, oled.width()/2, h-5, "water", (int)random(0,180), 0, 180);
  show_gauge(oled.width()/2,margin_top+h, oled.width()/2, h-5, "mani", (int)random(0,180), 0, 180);
}