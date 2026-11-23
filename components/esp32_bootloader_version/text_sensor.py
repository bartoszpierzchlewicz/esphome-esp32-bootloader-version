"""Text sensor that reports the ESP-IDF version of the 2nd-stage bootloader (ESP32 family only).

Platform: ``esp32_bootloader_version`` (``text_sensor``). It works only on the ESP32 family
(ESP32, ESP32-C3, ESP32-S3, ...) and needs an ESP-IDF that provides
``esp_ota_get_bootloader_description()``. It is rejected at validation time on other platforms.

Example::

    external_components:
      - source:
          type: local
          path: components
        components: [esp32_bootloader_version]

    text_sensor:
      - platform: esp32_bootloader_version
        name: "Bootloader"
      - platform: esp32_bootloader_version
        name: "Bootloader latest"
        kind: latest
"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC

CODEOWNERS = []

CONF_KIND = "kind"

esp32_bootloader_version_ns = cg.esphome_ns.namespace("esp32_bootloader_version")
ESP32BootloaderVersionTextSensor = esp32_bootloader_version_ns.class_(
    "ESP32BootloaderVersionTextSensor", text_sensor.TextSensor, cg.Component
)

CONFIG_SCHEMA = cv.All(
    text_sensor.text_sensor_schema(
        ESP32BootloaderVersionTextSensor,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        icon="mdi:chip",
    )
    .extend(
        {
            # "current": the bootloader installed on the device.
            # "latest": the ESP-IDF version of this firmware, i.e. the bootloader the device would get if it were updated.
            cv.Optional(CONF_KIND, default="current"): cv.one_of(
                "current", "latest", lower=True
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA),
    # This component only makes sense on the ESP32 family.
    cv.only_on_esp32,
)


async def to_code(config):
    var = await text_sensor.new_text_sensor(config)
    await cg.register_component(var, config)
    cg.add(var.set_latest(config[CONF_KIND] == "latest"))
