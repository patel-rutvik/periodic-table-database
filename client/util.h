#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <SD.h>
#include <TouchScreen.h>
#include <Fonts/FreeSansBold9pt7b.h>

namespace clientpins {
    // Pins and for the zoom in and out buttons.
    const uint8_t left_pin = 2;
    const uint8_t right_pin = 3;

    // the pins used to connect to the AdaFruit display
    const uint8_t sd_cs = 6;
    const uint8_t tft_cs = 10;
    const uint8_t tft_dc = 9;

    // analog pins for joystick axes
    const uint8_t joy_x_pin = A1;
    const uint8_t joy_y_pin = A0;

    // joystick button pin
    const uint8_t joy_button_pin = 8;
};

// constants related to the display
namespace displayconsts {
    // physical dimensions of the tft display
    const int16_t tft_width = 320;
    const int16_t tft_height = 240;

    // dimensions of the bottom "display" that has the FROM? or TO? prompt
    const int16_t msg_width = tft_width;
    const int16_t msg_height = 24;

    // the display window size for the map of edmonton
    const int16_t display_width = tft_width;
    const int16_t display_height = tft_height - msg_height;

    // amount of "padding" around the display, the cursor should
    // not move into the padding (will cause a map view nudge
    // unless we are near the boundary of the map itself)
    const int16_t padding = 2;


    // the minimum and maximum display coordinates we allow the
    // cursor to occupy, anything outside this should nudge the screen
    const int16_t min_x = padding;
    const int16_t max_x = display_width - padding;
    const int16_t min_y = padding;
    const int16_t max_y = display_height - padding;
};

Adafruit_ILI9341 tft = Adafruit_ILI9341(clientpins::tft_cs, clientpins::tft_dc);

#endif //UTIL_H