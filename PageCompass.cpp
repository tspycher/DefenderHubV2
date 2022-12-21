//
// Created by Thomas Spycher on 26.10.22.
//

#include "PageCompass.h"

String PageCompass::get_page_name() {
  return String("Compass");
}


bool PageCompass::needs_display_update() {
  if(millis() - last_update >= 1000) {
    last_update = millis();
    return true;
  }
  return false;
}


void PageCompass::update_display() {
  int x, y;
  int w, h;
  int r;
  int ticks_l = 4;
  x = 10;
  y = 20;
  w = oled.width() - (2 * x);
  h = oled.height() - (2 * y);
  r = h / 2;

  int course = (int)car.get_course();
  int altitude = (int)car.get_altitude();

  // reset old values
  oled.fillRect(x, y, w, h, 0x0000);

  // draw compass
  oled.drawCircle(x + w / 2, y + h / 2, r, GREEN);
  for (int i = 0; i <= 360; ++i) {
    // i is off 90degree to the left

    if (i % 10 == 0) {
      // draw little ticks
      float needle_x_out = cos(i * 0.0174533) * (float)r * -1.0;
      float needle_y_out = sin(i * 0.0174533) * (float)r;
      float needle_x_in = cos(i * 0.0174533) * (float)(r - ticks_l) * -1.0;
      float needle_y_in = sin(i * 0.0174533) * (float)(r - ticks_l);
      float needle_x_label = cos(i * 0.0174533) * (float)(r - 2 * ticks_l) * -1.0;
      float needle_y_label = sin(i * 0.0174533) * (float)(r - 2 * ticks_l);
      oled.drawLine(x + w / 2 + needle_x_in, y + h / 2 - needle_y_in, (x + w / 2) + needle_x_out, (y + h / 2) - needle_y_out, GREEN);

      // Print Windrose Markers
      oled.setCursor(x + w / 2 + needle_x_label - 2, y + h / 2 - needle_y_label - 2);
      switch (i) {
        case 0:
          oled.print("W");
          break;
        case 90:
          oled.print("N");
          break;
        case 180:
          oled.print("E");
          break;
        case 270:
          oled.print("S");
          break;

        default:
          break;
      }
    }

    // print course indicator
    if (i == (course + 90) % 360) {
      float course_x = cos(i * 0.0174533) * (float)(r - 4) * -1.0;
      float course_y = sin(i * 0.0174533) * (float)(r - 4);
      oled.fillCircle(x + w / 2 + course_x, y + h / 2 - course_y, 2, RED);
    }
  }

  // write course
  oled.setTextSize(2);
  draw_centre_string(String(course).c_str(), x + w / 2, y + h / 2 - 9);
  oled.setTextSize(1);
  draw_centre_string((String(altitude) + String(" m")).c_str(), x + w / 2, y + h / 2 + 9);
}