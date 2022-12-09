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
  show_gauge(0,20, oled.width()/2, 35, "Oil Temp", (int)random(0,180), 0, 180);
  show_gauge(0,65, oled.width()/2, 35, "Oil Pres", (int)random(0,180), 0, 180);

  show_gauge(64,20, oled.width()/2, 35, "Coolant", (int)random(0,180), 0, 180);

  show_gauge(64,65, oled.width()/2, 35, "manif.", (int)random(0,180), 0, 180);

  
}