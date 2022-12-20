//
// Created by Thomas Spycher on 05.08.22.
//

#ifndef DEFENDERHUB_EQUIPMENT_H
#define DEFENDERHUB_EQUIPMENT_H

#define ADDRESS_PCF8574_0 0x20
#define ADDRESS_PCF8574_1 0x21

#include <pcf8574.h>

#define EVENT_FINISH 0
#define EVENT_TURN_ON 1
#define EVENT_TURN_OFF 2
#define EVENT_TOGGLE 3
#define EVENT_EQUIPMENT_DISABLED 8
#define EVENT_ERROR 9

typedef void (*EquipmentHandler)(String name, int event);

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
  Equipment();
  void begin();

  void registerEquipmentHandler(EquipmentHandler handler);
  void toggle(int index);

  void turn_off(int index);
  void turn_on(int index);
  String get_name(int index);
  int get_num_relays();
  void check_button_states();

  bool is_on(int index);
  bool check_if_active(int index);

private:
  EquipmentHandler _handler;
  Relay *relays;
  int num_relays;
  PCF8574 *extend_relais;
  PCF8574 *extend_switches;
};


#endif //DEFENDERHUB_EQUIPMENT_H
