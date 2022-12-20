#include "defender.h"

float Defender::outside_temperature = 0.0;
float Defender::outside_humidity = 0.0;
signed int Defender::outside_rssi = 0;

float Defender::inside_temperature = 0.0;
float Defender::inside_humidity = 0.0;
signed int Defender::inside_rssi = 0;

Defender::Defender() : equipment(Equipment()), latitude(0.0), longitude(0.0), altitude(0.0), gpsspeed(0.0), course(0.0), satellites(999) {

}

void Defender::begin() {
  Serial.println("*** initalizing Defender Class");
  receiver = new RCSwitch();
  receiver->enableReceive(RX_PIN);  // Receiver on interrupt 0 => that is pin #2
  Serial.println("***** initalized 433mhz receiver");

  gps = new TinyGPSPlus();
  Serial1.begin(9600);
  Serial.println("***** initalized gps classes");

  equipment.begin();
  Serial.println("***** Equipment initialized");

  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  BLE.setEventHandler(BLEDiscovered, blePeripheralDiscoveredHandler);
  BLE.scan();
  Serial.println("***** registered ble event handler and startetd scanning");
}

void Defender::blePeripheralDiscoveredHandler(BLEDevice central) {
  bool debug = false;
  if(debug) {
    Serial.print("Discovered event from BLE Device with Mac: ");
    Serial.println(central.address());
  }

  if(central.hasManufacturerData() && central.hasAdvertisementData()) {
    // Check PDF From: https://www.bluetooth.com/specifications/assigned-numbers/
    // 0x0499 Ruuvi Innovations Ltd.
    // Decimal: 4 153
    uint8_t manufacturer[central.manufacturerDataLength()];
    central.manufacturerData(manufacturer, central.manufacturerDataLength());
    if((int)manufacturer[0] == 153 && (int)manufacturer[1] == 4) {
      Serial.print("Discovered event from RuuviTag with Mac: ");
      Serial.println(central.address());
    } else {
      if (debug) {
        Serial.print("Advertisement is from an unknown Device with Manufacturer Id: ");
        Serial.print((int)manufacturer[1], HEX);
        Serial.print((int)manufacturer[0], HEX);
        Serial.println();
      }
      return;
    }
  } else {
    if(debug) {
      Serial.print("Discovered Device with mac: ");
      Serial.print(central.address());
      Serial.println(" but does no advertised data or manufacturer data");
    }
     // no manufacturer Data available
    return;
  }

  // Load the Advertised Data and store the values
  int l = central.advertisementDataLength();
  uint8_t value[l];
  central.advertisementData(value, l);

  int l_payload = (int)value[3];
  int format = (int)value[7];
  int payload_start = 8;

  float temperature, humidity;
  signed int rssi;

  if (format == 5) { // Data Format 5 Protocol Specification (RAWv2)
    // https://github.com/ruuvi/ruuvi-sensor-protocols/blob/master/dataformat_05.md

    int16_t raw_temperature = (int16_t)value[payload_start]<<8 | (int16_t)value[payload_start+1];
    int16_t raw_humidity = (int16_t)value[payload_start+2]<<8 | (int16_t)value[payload_start+3];

    temperature = raw_temperature * 0.005; // Celcius
    humidity = raw_humidity * 0.0025; // Percent
    rssi = central.rssi();

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("C ");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("% ");
    Serial.print("RSSI: ");
    Serial.print(rssi);
    Serial.print("db");
    Serial.println();
  } else {
    Serial.print("Unknown Data Format from RuuviTag received: ");
    Serial.println(format);
    temperature = 0.0; // Celcius
    humidity = 0.0; // Percent
    rssi = central.rssi();
  }

  if(central.address() == "f2:40:f1:bc:69:e0") {
    inside_temperature = temperature;
    inside_humidity = humidity;
    inside_rssi = rssi;
  } else if(central.address() == "eb:4b:fa:41:fa:c5") { 
    outside_temperature = temperature;
    outside_humidity = humidity;
    outside_rssi = rssi;
  } else {
    Serial.print("This RuuviTag is unkown to me");
  }
}


void Defender::update(bool radio, bool gps, bool obd, bool ble) {
  int max_brightnes = 20;
  if(radio) {
    set_internal_rgb_led(max_brightnes,0,0);
    read_433();
  }
  if(gps) {
    set_internal_rgb_led(max_brightnes,max_brightnes,0);
    read_gps();
  }
  if(obd) {
    set_internal_rgb_led(0, max_brightnes, max_brightnes);
    read_obd();
  }
  if(ble) {
    set_internal_rgb_led(0,0,max_brightnes);
    read_ble();
  }
  set_internal_rgb_led(0,0,0);

}

float Defender::get_inside_temperature() {
  return inside_temperature;
  // return (float)random(-10, 39);
}

float Defender::get_outside_temperature() {
  return outside_temperature;
  // return (float)random(-10, 39);
}

float Defender::get_outside_humidity() {
  return outside_humidity;
}

float Defender::get_inside_humidity() {
  return inside_humidity;
}

double Defender::get_latitude() {
  return latitude;
}

double Defender::get_longitude() {
  return longitude;
}

double Defender::get_altitude() {
  return altitude;
}

double Defender::get_gpsspeed() {
  return gpsspeed;
}

double Defender::get_course() {
  return course;
}

int Defender::get_satellites() {
  return satellites;
}

void Defender::read_gps() {
  while (Serial1.available() > 0) {
    gps->encode(Serial1.read());
  }

  if (gps->satellites.isValid()) {
    satellites = gps->satellites.value();
  }

  if (gps->location.isValid()) {
    latitude = gps->location.lat();
    longitude = gps->location.lng();
  }

  if (gps->altitude.isValid()) {
    altitude = gps->altitude.value();
  }

  if (gps->course.isValid()) {
    course = gps->course.deg();
  }

  if (gps->speed.isValid()) {
    gpsspeed = gps->speed.kmph();
  }
}

void Defender::read_ble() {
  BLE.poll();
}

void Defender::read_obd() {
  
}

void Defender::read_433() {
  if (receiver->available()) {
    int bitlength = receiver->getReceivedBitlength();
    unsigned long value = receiver->getReceivedValue();
    int identifier = (value / 100000000);
    receiver->resetAvailable();

    switch(identifier) {
      case IDENTIFIER_TEMPERATURE:
        inside_temperature = (value % 100000000 / 1000000.0) - 20.0;
        outside_temperature = (value % 10000 / 100.0) - 20.0;
        Serial.print("433: Received Temperature Inside: ");
        Serial.print(inside_temperature);
        Serial.print(" Outside: ");
        Serial.println(outside_temperature);
        break;
      default:
        Serial.print("433: unknown package received. Value is:");
        Serial.print(value);
        Serial.print(" and identifier is: ");
        Serial.println(identifier);
        break;
    }
  }
}

void Defender::set_internal_rgb_led(int r, int g, int b) {
  /*WiFiDrv::analogWrite(25, r); // red
  WiFiDrv::analogWrite(26, g); // green
  WiFiDrv::analogWrite(27, b); // blue
  */
}
