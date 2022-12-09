//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageTemperature.h"

String PageTemperature::get_page_name() {
  return String("Temperature");
}

bool PageTemperature::needs_display_update() {
  if(inside_temperature == car.get_inside_temperature()) {
    if(outside_temperature == car.get_outside_temperature()) {
      return false;
    }
  }

  inside_temperature = car.get_inside_temperature();
  outside_temperature = car.get_outside_temperature();
  return true;
}

void PageTemperature::update_display() {
  // getting colors
  int inside_color, outside_color;
  if(inside_temperature >= 30.0) {
    inside_color = RED;
  } else if(inside_temperature >= 25.0) {
    inside_color = YELLOW;
  } else if(inside_temperature <= 5.0) {
    inside_color = BLUE;
  } else {
    inside_color = GREEN;
  }

  if(outside_temperature >= 30.0) {
    outside_color = RED;
  } else if(outside_temperature >= 25.0) {
    outside_color = YELLOW;
  } else if(outside_temperature <= 5.0) {
    outside_color = BLUE;
  } else {
    outside_color = GREEN;
  }

  // Show labels
  update_label("in", inside_temperature, 0,30,14, true, inside_color);
  update_label("out", outside_temperature, 0,70,14, true, outside_color);

  // Draw warning logos
  if(outside_temperature >= 30.0) {
    oled.drawBitmap(oled.width() - 48, 73, hot, 24, 24, RED);
  } else if(outside_temperature <= 5.0) {
    oled.drawBitmap(oled.width() - 48, 73, cold, 24, 24, BLUE);
  }
}