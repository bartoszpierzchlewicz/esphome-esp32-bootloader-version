#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace esp32_bootloader_version {

// ESP32 family only.
// kind "current": ESP-IDF version and build date of the installed 2nd-stage bootloader.
// kind "latest":  ESP-IDF version of the running firmware, i.e. the bootloader the device would get if it were
//                 updated, plus whether it differs from the installed one.
class ESP32BootloaderVersionTextSensor : public text_sensor::TextSensor, public Component {
 public:
  void set_latest(bool latest) { this->latest_ = latest; }
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::LATE; }

 protected:
  bool latest_{false};
};

}  // namespace esp32_bootloader_version
}  // namespace esphome
