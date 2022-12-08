//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageTemperature.h"

String PageTemperature::get_page_name() {
  return String("Temperature");
}

bool PageTemperature::needs_display_update() {
  return true;
}

void PageTemperature::update_display() {
  Serial.println("update Temperature Display");
  oled.fillScreen(BLUE);
}