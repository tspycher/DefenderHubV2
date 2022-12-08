#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 

#define BUTTON_SHORT_PRESS 1000
#define BUTTON_LONG_PRESS 3000
#define BUTTON_VERY_LONG_PRESS 5000

#define DC_PIN   4
#define CS_PIN   5
#define RST_PIN  3

#define BUTTON 1


#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SSD1351.h>
#include <Adafruit_GFX.h>
#include "Defender.h"
#include "DefenderMenu.h"

unsigned long looper = 0;
long int last_button_signal = 0;
long int last_button_press = 0;



Adafruit_SSD1351 oled = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);
Defender defender = Defender();
DefenderMenu menu = DefenderMenu(oled, defender);


void button_pressed() {
  //Serial.println("Butten Pressed");
  Serial.println("Switch Page by Interrupt");
  menu.show_message("Next", "Page");
  menu.switch_page_by_interrupt();
}


void button_debouncer() {
    if (menu.get_interrupt_switch_page())
        return;
    
    //noInterrupts();
    
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
  delay(500);
  Serial.begin(9600);
  delay(500); // let the IDE connect to the Serial port
  Serial.println("*** Starting up Defender HUB");

  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), button_debouncer, RISING);

  defender.begin();
  menu.begin();
}

void loop() {
  ++looper;
  if(menu.perform_interrupt_switch_page()) {
    Serial.println("Performed Page Switch");

  } else {
    Serial.println("Idle");
    delay(1000);
  }
}