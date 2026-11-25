"""Text sensor that reports the state of the 2nd-stage bootloader (ESP32 family only).

Platform: ``esp32_bootloader_version`` (``text_sensor``). The single state is ``up to date 5.5.6`` or
``installed 5.5.2, available 5.5.6``: the ESP-IDF version of the installed bootloader compared with the
ESP-IDF of the running firmware. It works only on the ESP32 family (ESP32, ESP32-C3, ESP32-S3, ...) and needs
an ESP-IDF that provides ``esp_ota_get_bootloader_description()``. It is rejected at validation time on other
platforms.

Example::

    external_components:
      - source: github://bartoszpierzchlewicz/esphome-esp32-bootloader-version@main
        components: [esp32_bootloader_version]

    text_sensor:
      - platform: esp32_bootloader_version
        # name defaults to "Bootloader"
"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_NAME, ENTITY_CATEGORY_DIAGNOSTIC

CODEOWNERS = []

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
    .extend({cv.Optional(CONF_NAME, default="Bootloader"): cv.string})
    .extend(cv.COMPONENT_SCHEMA),
    # This component only makes sense on the ESP32 family.
    cv.only_on_esp32,
)


async def to_code(config):
    var = await text_sensor.new_text_sensor(config)
    await cg.register_component(var, config)
