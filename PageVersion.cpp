//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageVersion.h"

String PageVersion::get_page_name() {
  return String("Version");
}


bool PageVersion::needs_display_update() {
  if(millis() - last_update >= 1000) {
    last_update = millis();
    return true;
  }
  return false;
}

void PageVersion::update_display() {
  int uptime_minutes = millis()/1000/60;
  int uptime_seconds = millis()/1000 % (uptime_minutes * 60);
  String uptime = String(uptime_minutes) + String(":") + String(uptime_seconds);

  update_label("Version", 1.0, 0, 20, 7, false);
  update_label("Buildtime", "unknown", 0, 45, 7, false);
  update_label("Copyright", "TSpycher", 0, 70, 7, false);
  update_label("Uptime", uptime, 0, 70, 7, false);

}