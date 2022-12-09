//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageGps.h"

String PageGps::get_page_name() {
  return String("GPS");
}


bool PageGps::needs_display_update() {
  return true;
}

void PageGps::update_display() {
  update_label("Latitude", car.get_latitude(), 0, 20, 7);
  update_label("Longitude", car.get_longitude(), 128 / 2, 20, 7);


  update_label("Alt(m)", car.get_altitude(), 0,60,7);
  update_label("Speed", car.get_gpsspeed(), 0,85,7);
  update_label("Course", car.get_course(), 128/2,60,7);
  update_label("Satellites", car.get_satellites(), 128/2,85,7);
}