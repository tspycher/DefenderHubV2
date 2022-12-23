//
// Created by Thomas Spycher on 26.10.22.
//

#include "PagePdc.h"

String PagePdc::get_page_name() {
  return String("PDC");
}


bool PagePdc::needs_display_update() {
  if(millis() - last_update >= 1000) {
    last_update = millis();
    return true;
  }
  return false;
}

int PagePdc::get_distance_left() {
  return (int)car.get_distance_left();
  //return 20;
}

int PagePdc::get_distance_right() {
  return (int)car.get_distance_right();
  //return 189;
}

void PagePdc::update_display() {
  oled.setTextColor(GREEN);
  oled.setTextSize(1);
  oled.fillRect(0, 25, oled.width(), oled.height()-35, 0x0000);
  int margin = 10;
  int top_margin = 40;
  int max_pixel_height = oled.height()-60;

  int pixel_left = ((float)max_pixel_height - 2) * (float)car.get_distance_approximation_left() + 2;
  int pixel_right = ((float)max_pixel_height -2) * (float)car.get_distance_approximation_right() + 2;

  int color_left;
  int color_right;

  if(car.get_distance_approximation_left() >= 0.75) {
    color_left = GREEN;
  } else if(car.get_distance_approximation_left() >= 0.5) {
    color_left = YELLOW;
  } else {
    color_left = RED;
  }

  if(car.get_distance_approximation_right() >= 0.75) {
    color_right = GREEN;
  } else if(car.get_distance_approximation_right() >= 0.5) {
    color_right = YELLOW;
  } else {
    color_right = RED;
  }

  // driver Side LEFT
  draw_centre_string((String(get_distance_left()) + String("cm")).c_str(), oled.width()*0.25, top_margin-9);
  oled.fillRect(margin, top_margin, oled.width()/2-(2*margin), pixel_left, color_left);

  // passenger Side RIGHT
  draw_centre_string((String(get_distance_right()) + String("cm")).c_str(), oled.width()*0.75, top_margin-9);
  oled.fillRect(oled.width() / 2 + margin, top_margin, oled.width()/2-(2*margin), pixel_right, color_right);

  if(car.get_distance_approximation_right() < 0.05 || car.get_distance_approximation_left() < 0.05) {
    oled.setTextColor(RED);
    oled.setTextSize(2);
    draw_centre_string("PANIC", oled.width()/2, oled.height()/2);
  } else if (car.get_distance_approximation_right() <= 0.2 || car.get_distance_approximation_left() <= 0.2) {
    oled.setTextColor(RED);
    oled.setTextSize(2);
    draw_centre_string("STOP", oled.width()/2, oled.height()/2);
  }
}