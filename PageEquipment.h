//
// Created by Thomas Spycher on 26.10.22.
//

#ifndef _DEFENDERHUB_PAGEEQUIPMENT_H
#define _DEFENDERHUB_PAGEEQUIPMENT_H

#include "Page.h"

class PageEquipment : public Page {
public:
    PageEquipment(Adafruit_SSD1351 &oled, Defender &car) : Page(oled, car) {};

    bool needs_display_update();
    void update_display();
    String get_page_name();

};

#endif //_DEFENDERHUB_PAGEEQUIPMENT_H
