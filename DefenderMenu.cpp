#include "DefenderMenu.h"


DefenderMenu::DefenderMenu(Adafruit_SSD1351 &oled, Defender &car) : oled(oled), car(car), num_pages(0), current_page_index(0), interrupt_switch_page(false) {

}

void DefenderMenu::begin() {
  Serial.println("*** initalizing Defender Menu Class");
  pages = new Page*[10];

  pages[num_pages] = new PageTemperature(oled, car);
  Serial.println("****** initialized Temperature Page");
  ++num_pages;

  pages[num_pages] = new PageEngine(oled, car);
  Serial.println("****** initialized Engine Page");
  ++num_pages;

  pages[num_pages] = new PageCompass(oled, car);
  Serial.println("****** initialized Compass Page");
  ++num_pages;
  
  pages[num_pages] = new PageGps(oled, car);
  Serial.println("****** initialized GPS Page");
  ++num_pages;

  pages[num_pages] = new PageVersion(oled, car);
  Serial.println("****** initialized Version Page");
  ++num_pages;

  oled.begin();
  oled.fillScreen(BLACK);
  welcome_screen(1);
  switch_page(0);
}

void DefenderMenu::welcome_screen(int delay_seconds) {
    oled.drawBitmap(0, 0, big_logo, oled.width(), oled.height(), GREEN);
    delay(delay_seconds*1000);
}

void DefenderMenu::draw_base_menu(const char title[]) {
  // header
  oled.fillScreen(BLACK);
  oled.setCursor(0, 3);
  oled.setTextColor(GREEN);
  oled.setTextSize(1);
  oled.print(title);
  oled.drawLine(0, 15, oled.width(), 15, GREEN);
  oled.drawBitmap(oled.width() - 34, 0, small_logo, 34, 15, GREEN);

  // footer
  int y = oled.height()-7;
  oled.setCursor(0, y);

  oled.print((String(current_page_index + 1) + String("/") + String(get_num_pages())).c_str());

  oled.setCursor(29, y);
  oled.print("Defender 110");
  oled.drawLine(0, y-3, oled.width(), y-3, GREEN);
}

void DefenderMenu::show_message(const char message[], const char message2[]) {
  int margin_top = 40;
  int margin_left = 5;
  int line_margin = 2;

  // blackout background of the popup
  oled.fillRect(margin_left, margin_top, oled.width()-(2*margin_left), oled.height()-(2*margin_top), BLACK);
  // top
  oled.drawFastHLine(margin_left+line_margin,margin_top+line_margin, oled.width()-(2*margin_left)-(2*line_margin), RED);
  // bottom
  oled.drawFastHLine(margin_left+line_margin, oled.height()-margin_top-line_margin, oled.width()-(2*margin_left)-(2*line_margin)+1, RED);
  // left
  oled.drawFastVLine(margin_left+line_margin,margin_top+line_margin, oled.height()-(2*margin_top)-(2*line_margin), RED);
  // right
  oled.drawFastVLine(oled.width()-margin_left-line_margin,margin_top+line_margin, oled.height()-(2*margin_top)-(2*line_margin)+1, RED);

  oled.setTextColor(RED);
  oled.setTextSize(1);
  oled.setCursor(margin_left*3, oled.height()/2-3-7);
  oled.print(message);
  oled.setCursor(margin_left*3, oled.height()/2-3+7);
  oled.print(message2);
}

Page* DefenderMenu::get_current_page() {
  return pages[current_page_index];
}

void DefenderMenu::update_display(bool force) {
  Page *p = get_current_page();
  
  if(p->needs_display_update() || force) {
    p->update_display();
  }
}

void DefenderMenu::switch_page(int page) {
  current_page_index = page % get_num_pages();
  draw_base_menu(get_current_page()->get_page_name().c_str());
  update_display(true);
}

void DefenderMenu::switch_page() {
  switch_page(current_page_index+1);
}

void DefenderMenu::switch_page_by_interrupt() {
    interrupt_switch_page = true;
}

bool DefenderMenu::perform_interrupt_switch_page() {
    if (interrupt_switch_page) {
        interrupt_switch_page = false;
        // fix for: sometimes the display "crashes" and is disabled or shows inverted colors...
        oled.enableDisplay(true);
        oled.invertDisplay(false);
        switch_page();
        return true;
    }
    return false;
}

bool DefenderMenu::display_update_required() {
  return get_current_page()->needs_display_update();
}

