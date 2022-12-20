//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageEquipment.h"

String PageEquipment::get_page_name() {
  return String("Equipment");
}


bool PageEquipment::needs_display_update() {
  return false;
}

void PageEquipment::update_display() {
  update_label("Equipment", "dingdong", 0,20,7, false);
}