#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <SD.h>
#include <TouchScreen.h>
#include <Fonts/FreeSansBold9pt7b.h>

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM  5  // can be a digital pin
#define XP  4  // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
#define MINPRESSURE   10
#define MAXPRESSURE 1000

int currentLayout = 1;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
int sidebar = 80;  // size of sidebar
bool touched = false;
int cardNum = 1;
bool flipped = false;
bool search = false;
uint8_t left_pushed;
uint8_t right_pushed;

void setup();
void textInit();
void welcomeScreen();
void drawButtons();
void getTouch();
void blankCard();
void backCard();
void minimalCard();
void classicCard();
void compactCard();
void drawCard();
void nextCard();
void assignValue(int index, String value);
String read_word(uint32_t timeout);
String read_value(uint32_t timeout);
bool checkTimeout(bool timeout, uint32_t time, uint32_t startTime);
void searchRequest();
void sendRequest();
void sendAck();
int getSearchResults(String arr[][2]);
void waitingScreen();
void noMatchScreen();
void failScreen();
int searchScreen(String arr[][2]);
void clientCom();


namespace clientpins {
    // Pins and for the zoom in and out buttons.
    const uint8_t left_pin = 3;
    const uint8_t right_pin = 2;

    // the pins used to connect to the AdaFruit display
    const uint8_t sd_cs = 6;
    const uint8_t tft_cs = 10;
    const uint8_t tft_dc = 9;
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