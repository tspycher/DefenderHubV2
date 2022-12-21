//
// Created by Thomas Spycher on 05.08.22.
//

#include "Equipment.h"

// CONFIGURING RELAYS
struct Relay relay0 = { 0, 0, "Radio", true, false, false };
struct Relay relay1 = { 1, 1, "Light1", false, false, true };
struct Relay relay2 = { 2, 2, "Light2", false, false, true };
struct Relay relay3 = { 3, 3, "Light3", false, false, true };
struct Relay relay4 = { 4, 4, "Light Inside", false, false, true };
struct Relay relay5 = { 5, 5, "Other", false, false, true };
struct Relay relay6 = { 6, 6, "This", false, false, true };
struct Relay relay7 = { 7, 7, "That", false, false, true };
Relay _relays[8] = { relay0, relay1, relay2, relay3, relay4, relay5, relay6, relay7 };


Equipment::Equipment()
  : relays(_relays), num_relays(8) {
}

void Equipment::begin() {
  Serial.println("*** initalizing Defender Equipment");
  Serial.print("****** number of equipment found: ");
  Serial.println(num_relays);

  extend_relais = new PCF8574(ADDRESS_PCF8574_0);
  extend_switches = new PCF8574(ADDRESS_PCF8574_1);

  // SEARCH FOR PCF8574
  for (int i = 0; i < 8; i++) {
    int address = PCF8574::combinationToAddress(i, false);
    if (PCF8574(address).read() != -1) {
      Serial.print("****** Found PCF8574: addr = 0x");
      Serial.println(address, HEX);
    }
  }
  Serial.println("****** PCF8574 initialization done ");

  for (int r; r < num_relays; ++r) {
    pinMode(*extend_relais, relays[r].relay_pin, OUTPUT);
    pinMode(*extend_switches, relays[r].switch_pin, INPUT_PULLUP);
    digitalWrite(*extend_relais, relays[r].relay_pin, HIGH);

    Serial.print("****** initalize Equipment: ");
    Serial.println(relays[r].name);
    //turn_off(r);
  }
}

void Equipment::registerEquipmentHandler(EquipmentHandler handler) {
  Serial.println("Handler registered");
  _handler = handler;
}


void Equipment::check_button_states() {
  int state;
  for (int r = 0; r < num_relays; ++r) {
    if (!relays[r].active) {
      continue;
    }

    state = digitalRead(*extend_switches, relays[r].switch_pin);

    if (relays[r].is_on_off_button) {
      if (state == LOW) {
        if (!is_on(r)) {
          turn_on(r);
        } else {
          turn_off(r);
        }
        delay(200);
      }

    } else {
      if (state == LOW) {
        if (!is_on(r)) {
          turn_on(r);
        }
      }

      if (state == HIGH) {
        if (is_on(r)) {
          turn_off(r);
        }
      }
    }
  }
}

int Equipment::get_num_relays() {
  return num_relays;
}

String Equipment::get_name(int index) {
  return relays[index].name;
}

bool Equipment::is_on(int index) {
  return relays[index].is_on;
}

bool Equipment::check_if_active(int index) {
  if (!relays[index].active) {
    Serial.print("Equipment ");
    Serial.print(get_name(index));
    Serial.println(" is disabled");
  }
  return relays[index].active;
}

void Equipment::turn_off(int index) {
  if (!check_if_active(index)) {
    if (_handler) {
      _handler(get_name(index), EVENT_EQUIPMENT_DISABLED);
      _handler(get_name(index), EVENT_FINISH);
    }
    return;
  }

  if (_handler)
    _handler(get_name(index), EVENT_TURN_OFF);
  Serial.print("Turning off: ");
  Serial.println(get_name(index));
  digitalWrite(*extend_relais, relays[index].relay_pin, HIGH);
  relays[index].is_on = false;
  if (_handler)
    _handler(get_name(index), EVENT_FINISH);
}

void Equipment::turn_on(int index) {
  if (!check_if_active(index)) {
    if (_handler) {
      _handler(get_name(index), EVENT_EQUIPMENT_DISABLED);
      _handler(get_name(index), EVENT_FINISH);
    }
    return;
  }

  if (_handler)
    _handler(get_name(index), EVENT_TURN_ON);
  Serial.print("Turning on: ");
  Serial.println(get_name(index));
  digitalWrite(*extend_relais, relays[index].relay_pin, LOW);
  relays[index].is_on = true;
  if (_handler)
    _handler(get_name(index), EVENT_FINISH);
}

void Equipment::toggle(int index) {
  if (_handler)
    _handler(get_name(index), EVENT_TOGGLE);

  if (is_on(index)) {
    turn_off(index);
  } else {
    turn_on(index);
  }
}
