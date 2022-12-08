//
// Created by Thomas Spycher on 02.08.22.
//

#ifndef _DEFENDERMENU_H
#define _DEFENDERMENU_H

#include "Colors.h"
#include <Adafruit_SSD1351.h>
#include "Defender.h"
#include "Logos.h"
#include "PageTemperature.h"
#include "PageVersion.h"

class DefenderMenu {
public:
  DefenderMenu(Adafruit_SSD1351 &oled, Defender &car);
  void begin();

  void switch_page();
  void switch_page(int);
  void switch_page_by_interrupt();
  int get_interrupt_switch_page() {return interrupt_switch_page;}

  bool perform_interrupt_switch_page();

  int get_num_pages() {return num_pages;}
  Page* get_current_page();

  void update_display(bool);
  void show_message(const char message[], const char message2[]); 

private:
  bool interrupt_switch_page;
  Page **pages;
  int num_pages;
  int current_page_index;

  void welcome_screen(int);
  void draw_base_menu(const char title[]);
  Adafruit_SSD1351& oled;
  Defender& car;
};
#endif //DEFENDERHUB_DEFENDERMENU_H
