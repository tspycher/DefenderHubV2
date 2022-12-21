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
  int row = 0;
  int col = 0;
  for (int i = 0; i < car.equipment.get_num_relays(); ++i) {
    if (i % 4 == 0) {
      row = 0;
      ++col;
    }
    update_label("Equipment", i, col * (oled.width() / 2), row * 10, 7, false);
    ++row;
  }
}