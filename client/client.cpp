#include "util.h"
#include "lcd_image.h"


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


void setup() {
  // initialize Arduino
  init();

  // initialize zoom pins
  pinMode(clientpins::left_pin, INPUT_PULLUP);
  pinMode(clientpins::right_pin, INPUT_PULLUP);

  // initialize joystick pins and calibrate centre reading
  pinMode(clientpins::joy_button_pin, INPUT_PULLUP);
  // x and y are reverse because of how our joystick is oriented
  

  // initialize serial port
  Serial.begin(9600);
  Serial.flush();  // get rid of any leftover bits


  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_WHITE);

  // initialize SD card
  if (!SD.begin(clientpins::sd_cs)) {
      Serial.println("Initialization has failed. Things to check:");
      Serial.println("* Is a card inserted properly?");
      Serial.println("* Is your wiring correct?");
      Serial.println("* Is the chipSelect pin the one for your shield or module?");
      while (1) {}  // nothing to do here, fix the card issue and retry
  }
}

void textInit() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The textInit function takes in no paramaters.

It does not return any parameters.

This function is responsible for taking care of all the initial duties of
correctly setting up the text and formatting it accordingly. The test used in
this exercise is all in one colour and size.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setFont(&FreeSansBold9pt7b);
}

void welcomeScreen() {
    tft.setCursor(displayconsts::tft_width/3, displayconsts::tft_height/4);
    textInit();
    tft.print("Welcome!");
    tft.setCursor(displayconsts::tft_width/6, displayconsts::tft_height/4 + 50);
    tft.print("Periodic Table Database");
    tft.setCursor(displayconsts::tft_width/6 + 10, displayconsts::tft_height/2 + 40);
    //tft.setFont(2);
    tft.print("By: Rutvik and Kaden");
    delay(3000);
    tft.fillScreen(ILI9341_BLACK);
}


void drawButtons() {
    tft.setCursor(displayconsts::tft_width - (sidebar/2) - 10, displayconsts::tft_height/4);
    tft.setTextColor(ILI9341_WHITE);
    tft.fillRect(displayconsts::tft_width - sidebar + 1, 0, sidebar - 1, displayconsts::tft_height/2 - 1, tft.color565(0, 0, 0));
    if (currentLayout == 1) {
        tft.print("L1");
        tft.drawRect(displayconsts::tft_width - sidebar + 1, 0, sidebar - 1, displayconsts::tft_height/2 - 1, tft.color565(0, 255, 0));
    } else if (currentLayout == 2) {
        tft.print("L2");
        tft.drawRect(displayconsts::tft_width - sidebar + 1, 0, sidebar - 1, displayconsts::tft_height/2 - 1, tft.color565(0, 0, 255));
    } else if (currentLayout == 3) {
        tft.print("L3");
        tft.drawRect(displayconsts::tft_width - sidebar + 1, 0, sidebar - 1, displayconsts::tft_height/2 - 1, tft.color565(255, 0, 0));
    }

    tft.setCursor(displayconsts::tft_width - (sidebar/2) - 30, 3*displayconsts::tft_height/4);
    tft.print("Search");
    tft.drawRect(displayconsts::tft_width - sidebar + 1, displayconsts::tft_height/2, sidebar - 1, displayconsts::tft_height/2 - 1, tft.color565(255, 255, 0));
}

bool getTouch() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The getTouch function takes no paramaters:

It does not return any parameters.

The point of this function is to...
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    TSPoint touch = ts.getPoint();
    if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
        return false;
    }
    // mapping to the screen, same implementation as we did in class
    int16_t touched_x = map(touch.y, TS_MINY, TS_MAXY, displayconsts::tft_width, 0);
    int16_t touched_y = map(touch.x, TS_MINX, TS_MAXX, 0, displayconsts::tft_height);
    if (touched_x < displayconsts::tft_width - 48) {
        delay(1000);
        //flip card here...
    // Check if a button is pressed and update the appropriate one.
    } else if (touched_x >= displayconsts::tft_width - sidebar) {
        if (touched_y <= displayconsts::tft_height/2 - 2) {
            //increment layout
            currentLayout++;
            if (currentLayout > 3) {
                currentLayout = 1;
            }
            //updateButtons(0);
            drawButtons();
            return true;
        } else if (touched_y >= displayconsts::tft_height/2 + 2) {
            delay(3000);
            return true;
            //search protocol goes here...
        }
        delay(100);
    }
}


void blankCard() {
    //tft.fillScreen(ILI9341_BLACK);

    // blank white card
    tft.fillRect( 0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, ILI9341_WHITE);

    /* draw buttons */
    drawButtons();
}

void minimalCard(String name, String number, String symbol, String protons, String neutrons, String weight) {
    blankCard();
    tft.setTextColor(ILI9341_BLACK);
    tft.setCursor(0, 15);
    tft.print(number);
    tft.setCursor(0, 70);
    tft.setTextSize(4);
    tft.print(symbol);
    tft.setCursor((displayconsts::tft_width - sidebar)/3, displayconsts::tft_height/5);
    tft.setTextSize(1);
    tft.print(name);
    tft.setCursor((displayconsts::tft_width - sidebar)/3 + 5, displayconsts::tft_height/3);
    tft.print(weight);
    tft.setCursor((displayconsts::tft_width - sidebar)/3, displayconsts::tft_height - 10);
    tft.print("minimal layout");
    while(!touched) {
        touched = getTouch();
    }
    touched = false;
}

void classicCard(String name, String number, String symbol, String protons, String neutrons, String weight) {
    blankCard();
    tft.setTextColor(ILI9341_BLACK);
    tft.setCursor((displayconsts::tft_width - sidebar)/3, displayconsts::tft_height - 10);
    tft.print("classic layout");
    while(!touched) {
        touched = getTouch();
    }
    touched = false;
}

void compactCard(String name, String number, String symbol, String protons, String neutrons, String weight) {
    blankCard();
    tft.setTextColor(ILI9341_BLACK);
    tft.setCursor((displayconsts::tft_width - sidebar)/3, displayconsts::tft_height - 10);
    tft.print("compact layout");
    while(!touched) {
        touched = getTouch();
    }
    touched = false;
}


void drawCard(String name, String number, String symbol, String protons, String neutrons, String weight) {
    if (currentLayout == 1) {
        minimalCard(name, number, symbol, protons, neutrons, weight);
    } else if (currentLayout == 2) {
        classicCard(name, number, symbol, protons, neutrons, weight);
    } else if (currentLayout == 3) {
        compactCard(name, number, symbol, protons, neutrons, weight);
    }
}


int main() {
    setup();  // setup program
    welcomeScreen();  // display welcome screen
    //blankCard();  // create blank card

    //minimalCard("Hydrogen", 1);  // example minimal layout
    while (true) {
        drawCard("Hydrogen", "1", "H", "1", "1", "weight");
    }
    return 0;
}