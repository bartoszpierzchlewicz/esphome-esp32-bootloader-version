#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace esp32_bootloader_version {

// ESP32 family only.
// Compares the ESP-IDF version of the installed 2nd-stage bootloader with the ESP-IDF of the running firmware
// (the bootloader the device would get if it were updated) and publishes one state:
// "up to date X" or "installed X, available Y".
class ESP32BootloaderVersionTextSensor : public text_sensor::TextSensor, public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::LATE; }
};

}  // namespace esp32_bootloader_version
}  // namespace esphome
