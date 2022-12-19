//
// Created by Thomas Spycher on 05.08.22.
//

#ifndef DEFENDERHUB_EQUIPMENT_H
#define DEFENDERHUB_EQUIPMENT_H

#define ADDRESS_PCF8574_0 0x20
#define ADDRESS_PCF8574_1 0x21

#include <pcf8574.h>
#include <Adafruit_SSD1351.h>
#include "DefenderMenu.h"

struct Relay {
    int relay_pin;
    int switch_pin;
    String name;
    bool active;
    bool is_on;
    bool is_on_off_button;
};

class Equipment {
public:
  Equipment(DefenderMenu &menu, Relay *relays, int num_relays);
  void begin();

  //void turn_off(int index, void (*func)(char*, int));
  //void turn_on(int index, void (*func)(char*, int));
  void toggle(int index);

  void turn_off(int index);
  void turn_on(int index);
  String get_name(int index);
  int get_num_relays();
  void check_button_states();

  bool is_on(int index);
  bool check_if_active(int index);

private:

  Relay *relays;
  int num_relays;
  PCF8574 *extend_relais;
  PCF8574 *extend_switches;

  DefenderMenu& menu;
};


#endif //DEFENDERHUB_EQUIPMENT_H
