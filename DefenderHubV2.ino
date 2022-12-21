#define PSEUDO_THREADS 3

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

#define BUTTON_SHORT_PRESS 1000
#define BUTTON_LONG_PRESS 3000
#define BUTTON_VERY_LONG_PRESS 5000

#define DC_PIN 4
#define CS_PIN 5
#define RST_PIN 3

#define BUTTON 1

//#include "Properties.h"
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SSD1351.h>
#include <Adafruit_GFX.h>
#include "Defender.h"
#include "DefenderMenu.h"
#include "Equipment.h"
#include "Colors.h"

bool devmode = true;
bool iotsupport = false;

unsigned long looper = 0;
long int last_button_signal = 0;
long int last_button_press = 0;

Adafruit_SSD1351 oled = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);
Defender defender = Defender();
DefenderMenu menu = DefenderMenu(oled, defender);

void equipment_button_pressed(String name, int event) {
  if(menu.get_current_page()->get_page_name() == "Equipment") {
    menu.update_display();
    return;
  }

  switch (event) {
    case EVENT_FINISH:
      delay(2000);
      menu.redraw_display();
      break;
    case EVENT_TURN_ON:
      menu.show_message(String("Equipment ON").c_str(), name.c_str(), BLUE);
      break;
    case EVENT_TURN_OFF:
      menu.show_message(String("Equipment OFF").c_str(), name.c_str(), BLUE);
      break;
    case EVENT_EQUIPMENT_DISABLED:
      menu.show_message(String("Equipment is DISABLED").c_str(), name.c_str(), RED);
      break;
    case EVENT_ERROR:
      menu.show_message(String("Equipment ERROR").c_str(), name.c_str(), RED);
      break;
    default:
      Serial.print("Unknown Equipment Event: ");
      Serial.println(event);
  }
}

void button_pressed() {
  Serial.println("Switch Page by Interrupt");
  menu.show_message("Next", "Page", GREEN);
  delay(1000);

  menu.switch_page_by_interrupt();
}


void button_debouncer() {
  if (menu.get_interrupt_switch_page())
    return;
  int diff = millis() - last_button_signal;
  int diff2 = millis() - last_button_press;

  last_button_signal = millis();

  if (diff >= 50 and diff <= BUTTON_SHORT_PRESS) {
    if (diff2 >= BUTTON_SHORT_PRESS / 4) {
      button_pressed();
      last_button_press = millis();
    }
  }
}

void setup() {
  if (devmode) {
    Serial.begin(9600);
    while (!Serial)
      ;
  }
  Serial.println("* Starting up Defender HUB");

  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), button_debouncer, RISING);

  defender.begin();
  menu.begin();
  defender.equipment.registerEquipmentHandler(equipment_button_pressed);
  Serial.println("* Starting completed");
}

void loop_thread0() {
  if (looper % 2000 == 0 && devmode) {
    defender.debug_print();
  }
  if (menu.perform_interrupt_switch_page()) {
    Serial.println("Performed Page Switch");
  }
}

void loop_thread1() {
  if (menu.display_update_required()) {
    menu.update_display(true);
  }
}

/*void test_equipment() {
    for(int i = 0; i < 8; ++i) {
    equipment.toggle(i);
    delay(200);
  }
}*/

void loop_thread2() {
  bool radio, gps, obd, ble;
  radio = false;
  gps = true;
  obd = false;
  ble = true;

  menu.show_status_indicator(GREEN);
  defender.update(radio, gps, obd, ble);
  menu.hide_status_indicator();
}

void loop() {
  defender.equipment.check_button_states();
  ++looper;

  int thread = looper % PSEUDO_THREADS;
  // Loop-Switching to simulate multi threading
  switch (thread) {
    case 0:
      loop_thread0();
      break;
    case 1:
      loop_thread1();
      break;
    case 2:
      loop_thread2();
      break;
    default:
      Serial.println("No thread registered for id: " + thread);
      break;
  }
}