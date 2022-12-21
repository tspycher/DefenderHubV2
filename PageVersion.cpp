//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageVersion.h"

String PageVersion::get_page_name() {
  return String("Version");
}


bool PageVersion::needs_display_update() {
  return false;
}

void PageVersion::update_display() {
  update_label("Version", 1.0, 0, 20, 7, false);
  update_label("Buildtime", "unknown", 0, 45, 7, false);
  update_label("Copyright", "TSpycher", 0, 70, 7, false);
}