#include "esp32_bootloader_version.h"
#include "esphome/core/log.h"

#include <cstdio>
#include <cstring>
#include "esp_ota_ops.h"
#include "esp_system.h"

namespace esphome {
namespace esp32_bootloader_version {

static const char *const TAG = "esp32_bootloader_version";

static const char *skip_v(const char *s) { return (s != nullptr && s[0] == 'v') ? s + 1 : s; }

// Copies the leading "X.Y.Z" of a version string (without the "v" prefix and any git suffix).
static void core_version(const char *in, char *out, size_t n) {
  const char *s = skip_v(in);
  size_t i = 0;
  while (s[i] != '\0' && (s[i] == '.' || (s[i] >= '0' && s[i] <= '9')) && i + 1 < n) {
    out[i] = s[i];
    i++;
  }
  out[i] = '\0';
}

void ESP32BootloaderVersionTextSensor::setup() {
  esp_bootloader_desc_t desc;
  // A bootloader built by an older ESP-IDF has no descriptor; the call then fails.
  esp_err_t err = esp_ota_get_bootloader_description(nullptr, &desc);
  char buf[96];
  if (!this->latest_) {
    if (err == ESP_OK) {
      if (desc.date_time[0] != 0)
        snprintf(buf, sizeof(buf), "ESP-IDF %s (%s)", skip_v(desc.idf_ver), desc.date_time);
      else
        snprintf(buf, sizeof(buf), "ESP-IDF %s", skip_v(desc.idf_ver));
    } else {
      snprintf(buf, sizeof(buf), "unknown (%s)", esp_err_to_name(err));
    }
  } else {
    char app_v[24];
    core_version(esp_get_idf_version(), app_v, sizeof(app_v));
    if (err != ESP_OK) {
      snprintf(buf, sizeof(buf), "ESP-IDF %s", app_v);
    } else {
      char boot_v[24];
      core_version(desc.idf_ver, boot_v, sizeof(boot_v));
      if (strcmp(app_v, boot_v) == 0)
        snprintf(buf, sizeof(buf), "ESP-IDF %s (up to date)", app_v);
      else
        snprintf(buf, sizeof(buf), "ESP-IDF %s (update available)", app_v);
    }
  }
  this->publish_state(buf);
}

void ESP32BootloaderVersionTextSensor::dump_config() {
  LOG_TEXT_SENSOR("", this->latest_ ? "ESP32 bootloader latest" : "ESP32 bootloader version", this);
  ESP_LOGCONFIG(TAG, "  State: %s", this->state.c_str());
}

}  // namespace esp32_bootloader_version
}  // namespace esphome
