#define PSEUDO_THREADS 3

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 

#define BUTTON_SHORT_PRESS 1000
#define BUTTON_LONG_PRESS 3000
#define BUTTON_VERY_LONG_PRESS 5000

#define DC_PIN   4
#define CS_PIN   5
#define RST_PIN  3

#define BUTTON 1

#include "Properties.h"
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SSD1351.h>
#include <Adafruit_GFX.h>
#include "Defender.h"
#include "DefenderMenu.h"
#include "Equipment.h"


bool devmode = true;
bool iotsupport = false;

unsigned long looper = 0;
long int last_button_signal = 0;
long int last_button_press = 0;

// CONFIGURING RELAYS
struct Relay relay0 = {0,0, "Radio", true, false, false};
struct Relay relay1 = {1,1, "Light1", false, false, true};
struct Relay relay2 = {2,2, "Light2", false, false, true};
struct Relay relay3 = {3,3, "Light3", false, false, true};
struct Relay relay4 = {4,4, "Light Inside", false, false, true};
struct Relay relay5 = {5,5, "Other", false, false, true};
struct Relay relay6 = {6,6, "This", false, false, true};
struct Relay relay7 = {7,7, "That", false, false, true};
Relay relays[] = {relay0,relay1, relay2, relay3, relay4, relay5, relay6, relay7};


Adafruit_SSD1351 oled = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);
Defender defender = Defender();
DefenderMenu menu = DefenderMenu(oled, defender);
Equipment equipment = Equipment(menu, relays, 8);


void button_pressed() {
  //Serial.println("Butten Pressed");
  Serial.println("Switch Page by Interrupt");
  menu.show_message("Next", "Page");
  menu.switch_page_by_interrupt();
}


void button_debouncer() {
  if (menu.get_interrupt_switch_page())
      return;    
  int diff = millis() - last_button_signal;
  int diff2 = millis() - last_button_press;

  last_button_signal = millis();

  if (diff >= 50 and diff <= BUTTON_SHORT_PRESS) {
      if (diff2 >= BUTTON_SHORT_PRESS/4) {
          button_pressed();
          last_button_press = millis();
      }
  }
}

void setup() {
  if(devmode) {
    Serial.begin(9600);
    while (!Serial);
  }
  Serial.println("* Starting up Defender HUB");

  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), button_debouncer, RISING);

  if(iotsupport) {
    // Connect to Arduino IoT Cloud
    Serial.println("*** initializing Arduino Cloud Connection");
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo();
  }

  defender.begin();
  menu.begin();
  equipment.begin();
  Serial.println("* Starting completed");

}

void loop_thread0() {
  if(looper % 20 == 0) {
    Serial.println("I'm Alive");
  }
  if(menu.perform_interrupt_switch_page()) {
    Serial.println("Performed Page Switch");
  }
}

void loop_thread1() {
    if(menu.display_update_required()) {
      menu.update_display(true);
    }
}

void test_equipment() {
    for(int i = 0; i < 8; ++i) {
    equipment.toggle(i);
    delay(200);
  }
}

void loop_thread2() {
  bool radio, gps, obd, ble;
  radio = true;
  gps = true;
  obd = true;
  ble = true;

  menu.show_status_indicator(GREEN);
  defender.update(radio, gps, obd, ble);
  menu.hide_status_indicator();

}

void loop() {
  if(iotsupport) {
    ArduinoCloud.update();
  }
  equipment.check_button_states();
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
          Serial.println("No thread registered for id: "+thread);
          break;
  }
  delay(500);
}