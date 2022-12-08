//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageVersion.h"

String PageVersion::get_page_name() {
  return String("Version");
}


bool PageVersion::needs_display_update() {
  return true;
}

void PageVersion::update_display() {
  Serial.println("update Version Display");
  oled.fillScreen(RED);
}