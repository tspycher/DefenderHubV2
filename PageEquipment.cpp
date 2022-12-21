//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageEquipment.h"

String PageEquipment::get_page_name() {
  return String("Equipment");
}


bool PageEquipment::needs_display_update() {
  if (last_change != car.equipment.get_last_change()) {
    last_change = car.equipment.get_last_change();
    return true;
  }
  return false;
}

void PageEquipment::update_display() {
  int row = 0;
  int col = 0;
  int x, y, color;
  String value, label;

  for (int i = 0; i < car.equipment.get_num_relays(); ++i) {
    if (i > 0 && i % 4 == 0) {
      row = 0;
      ++col;
    }
    x = col * (oled.width() / 2);
    y = row * 22 + 25;
    if (car.equipment.is_on(i)) {
      value = "ON";
      color = BLUE;
    } else {
      value = "OFF";
      color = RED;
    }
    if (car.equipment.get_name(i).length() > 9) {
      label = car.equipment.get_name(i).substring(0, 9) + String(".");
    } else {
      label = car.equipment.get_name(i);
    }

    update_label(label.c_str(), value, x, y, 7, false, color);
    ++row;
  }
}