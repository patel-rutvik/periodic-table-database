#include "util.h"
#include "types.h"


void setup() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
This is the setup function which is responsible for setting up the touchscreen
display on the Arduino.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  // initialize Arduino
  init();

  // initialize zoom pins
  pinMode(clientpins::left_pin, INPUT_PULLUP);
  pinMode(clientpins::right_pin, INPUT_PULLUP);

  left_pushed = (digitalRead(clientpins::left_pin) == HIGH);
  right_pushed = (digitalRead(clientpins::right_pin) == HIGH);
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
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The welcomeScreen function takes in no paramaters.

It does not return any parameters.

This function is responsible for displaying a simple welcome.screen once the
program starts.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    tft.setCursor(displayconsts::tft_width/3, displayconsts::tft_height/4);
    textInit();
    tft.print("Welcome!");
    tft.setCursor(displayconsts::tft_width/6, displayconsts::tft_height/4 + 50);
    tft.print("Periodic Table Database");
    tft.setCursor(displayconsts::tft_width/6 + 10,
                  displayconsts::tft_height/2 + 40);
    tft.print("By: Rutvik and Kaden");
    delay(5000);
    tft.fillScreen(ILI9341_BLACK);
}


void drawButtons() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The drawButtons function takes in no paramaters.

It does not return any parameters.

This function is responsible for drawing the buttons with proper labelling onto
the screen.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    tft.setTextSize(1);
    tft.setCursor(displayconsts::tft_width - (sidebar/2) - 10,
                  displayconsts::tft_height/4);
    tft.setTextColor(ILI9341_WHITE);
    tft.fillRect(displayconsts::tft_width - sidebar + 1, 0, sidebar - 1,
                 displayconsts::tft_height/2 - 1, tft.color565(0, 0, 0));
    if (currentLayout == 1) {  // layout 1: minimal
        tft.print("L1");
        tft.drawRect(displayconsts::tft_width - sidebar + 1, 0, sidebar - 1,
                     displayconsts::tft_height/2 - 1, tft.color565(0, 255, 0));
    } else if (currentLayout == 2) {  // layout 2: classic
        tft.print("L2");
        tft.drawRect(displayconsts::tft_width - sidebar + 1, 0, sidebar - 1,
                     displayconsts::tft_height/2 - 1, tft.color565(0, 0, 255));
    } else if (currentLayout == 3) {  // layout 3: compact
        tft.print("L3");
        tft.drawRect(displayconsts::tft_width - sidebar + 1, 0, sidebar - 1,
                     displayconsts::tft_height/2 - 1, tft.color565(255, 0, 0));
    }
    tft.setCursor(displayconsts::tft_width - (sidebar/2) - 30,
                  3*displayconsts::tft_height/4);
    tft.print("Search");
    tft.drawRect(displayconsts::tft_width - sidebar + 1,
                 displayconsts::tft_height/2, sidebar - 1,
                 displayconsts::tft_height/2 - 1, tft.color565(255, 255, 0));
}


void getTouch() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The getTouch function takes no paramaters:

It does not return any parameters.

The point of this function is to constantly read the input devices on the
Arduino and update the states accordingly.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // Read buttons
    left_pushed = (digitalRead(clientpins::left_pin) == HIGH);
    right_pushed = (digitalRead(clientpins::right_pin) == HIGH);
    delay(100);  // Button bounce
    if (left_pushed && cardNum > 1) {
        cardNum--;  // decrement card number
        nextCard();  // go to next card
        return;
    } else if (right_pushed && cardNum < 118) {
        cardNum++;  // increment card number
        nextCard();  // go to next card
        return;
    } else if (right_pushed && cardNum == 118) {
        cardNum = 1;  // reset card number
        nextCard();  // go to next card
        return;
    } else if (left_pushed && cardNum == 1) {
        cardNum = 118;  // max out card number
        nextCard();  // go to next card
        return;
    }

    TSPoint touch = ts.getPoint();
    if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
        return;  // if no touch, exit function
    }
    // mapping to the screen, same implementation as we did in class
    int16_t touched_x = map(touch.y, TS_MINY, TS_MAXY,
                         displayconsts::tft_width, 0);
    int16_t touched_y = map(touch.x, TS_MINX, TS_MAXX, 0,
                         displayconsts::tft_height);
    if (touched_x < displayconsts::tft_width - 48) {
        // flip card if screen is pressed
        if (!flipped) {
            flipped = true;
        } else {
            flipped = false;
        }
        drawCard();
    // Check if a button is pressed and update the appropriate one.
    } else if (touched_x >= displayconsts::tft_width - sidebar) {
        if (touched_y <= displayconsts::tft_height/2 - 2) {
            // increment layout
            currentLayout++;
            if (currentLayout > 3) {
                currentLayout = 1;
            }
            drawButtons();
            drawCard();
        } else if (touched_y >= displayconsts::tft_height/2 + 2) {
            // Search
            search = true;
            nextCard();
            search = false;
        }
        delay(100);
    }
}


void blankCard() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The blankCard function takes in no paramaters.

It does not return any parameters.

This function is responsible for drawing a blank card on the screen.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /* Colouring screen based on element type */
    if (element.type == "Nonmetal") {  // Oxygen
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(128, 235, 128));
    } else if (element.type == "Transition Metal") {  // Gold
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(255, 120, 120));
    } else if (element.type == "Noble Gas") {  // Helium
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(173, 216, 250));
    } else if (element.type == "Alkali Metal") {  // Lithium
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(255, 180, 98));
    } else if (element.type == "Alkaline Earth Metal") {  // Calcium
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(0, 140, 140));
    } else if (element.type == "Halogen") {  // Fluorine
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(50, 255, 241));
    } else if (element.type == "Lanthanide") {
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(255, 255, 255));
    } else if (element.type == "Metal") {  // Tin
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(250, 250, 100));
    } else if (element.type == "Metalloid") {  // Boron
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(197, 139, 231));
    } else if (element.type == "Actinide") {
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(255, 255, 255));
    } else if (element.type == "Transactinide") {
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(255, 255, 255));
    } else {
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar,
                     displayconsts::tft_height, tft.color565(255, 255, 255));
    }
    /* draw buttons */
    drawButtons();
}


void backCard() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The backCard function takes in no paramaters.

It does not return any parameters.

This function is responsible for updating the screen and drawing the back of the
current card.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // Display back of card
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 15);
    tft.print("Neutrons:");
    tft.setCursor(85, 15);
    tft.print(element.neutrons);
    tft.setCursor(0, 35);
    tft.print("Protons:");
    tft.setCursor(75, 35);
    tft.print(element.protons);
    tft.setCursor(0, 55);
    tft.print("Electrons:");
    tft.setCursor(90, 55);
    tft.print(element.electrons);
    tft.setCursor(0, 75);
    tft.print("Period:");
    tft.setCursor(60, 75);
    tft.print(element.period);
    tft.setCursor(0, 95);
    tft.print("Group:");
    tft.setCursor(60, 95);
    tft.print(element.group);
    tft.setCursor(0, 115);
    tft.print("Phase:");
    tft.setCursor(60, 115);
    tft.print(element.phase);
    tft.setCursor(0, 135);
    tft.print("Founder:");
    tft.setCursor(80, 135);
    tft.print(element.founder);
    tft.setCursor(0, 155);
    tft.print("Natural:");
    tft.setCursor(70, 155);
    tft.print(element.natural);
    tft.setCursor(0, 175);
    tft.print("Type:");
    tft.setCursor(50, 175);
    tft.print(element.type);
    tft.setCursor(0, 195);
    tft.print("Radius:");
    tft.setCursor(70, 195);
    tft.print(element.atomRadius);
    tft.setCursor(0, 215);
    tft.print("melting point:");
    tft.setCursor(118, 215);
    tft.print(element.mp);
    tft.setCursor(0, 235);
    tft.print("boiling point:");
    tft.setCursor(115, 235);
    tft.print(element.bp);
}


void minimalCard() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The minimalCard function takes in no paramaters.

It does not return any parameters.

This function is responsible for drawing the minimal layout onto the screen.
This requires us to clear the screen, and draw the text in the correct positions
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    blankCard();  // reset the card
    if (!flipped) {  // draw front of card
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(0, 25);
        tft.print(element.atomNum);
        tft.setCursor(0, 90);
        tft.setTextSize(4);
        tft.print(element.symbol);
        tft.setCursor(0, 130);
        tft.setTextSize(2);
        tft.print(element.name);
    } else {
        backCard();  // draw back of card
    }
}


void classicCard() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The classicCard function takes in no paramaters.

It does not return any parameters.

This function is responsible for drawing the classic layout onto the screen.
This requires us to clear the screen, and draw the text in the correct positions
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    blankCard();  // reset the card
    if (!flipped) {  // draw front of the card
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(0, 25);
        tft.print(element.atomNum);
        tft.setTextSize(1);
        tft.setCursor(205, 15);
        tft.print(element.electroNeg);
        tft.setCursor(0, 90);
        tft.setTextSize(4);
        tft.print(element.symbol);
        tft.setCursor(0, 130);
        tft.setTextSize(2);
        tft.print(element.name);
        tft.setTextSize(1);
        tft.setCursor(0, 160);
        tft.print(element.mass);
    } else {
        backCard();  // draw back of card
    }
}


void compactCard() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The compactCard function takes in no paramaters.

It does not return any parameters.

This function is responsible for drawing the classic layout onto the screen.
This requires us to clear the screen, and draw the text in the correct positions
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    blankCard();  // reset the card
    if (!flipped) {  // draw front of the card
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(1);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(0, 20);
        tft.print(element.atomNum);
        tft.setTextSize(1);
        tft.setCursor(205, 15);
        tft.print(element.electroNeg);
        tft.setCursor(0, 50);
        tft.setTextSize(2);
        tft.print(element.symbol);
        tft.setCursor(0, 80);
        tft.setTextSize(1);
        tft.print(element.name);
        tft.setCursor(0, 110);
        tft.print(element.mass);
        tft.setCursor(0, 140);
        tft.print(element.phase);
        tft.setCursor(0, 170);
        tft.print(element.type);
        tft.setCursor(0, 200);
        tft.print(element.founder);
        tft.setCursor(0, 230);
        tft.print(element.year);
    } else {
        backCard();  // draw back of card
    }
}


void drawCard() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The drawCard function takes in no paramaters.

It does not return any parameters.

This function is responsible for calling the correct layout by checking the
currentLayout variable.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    if (currentLayout == 1) {
        minimalCard();
    } else if (currentLayout == 2) {
        classicCard();
    } else if (currentLayout == 3) {
        compactCard();
    }
}

void nextCard() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The nextCard function takes in no paramaters.

It does not return any parameters.

This function is responsible for calling the client communication, and then
changing the card number and updating the screen.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // Gets next card from server
    clientCom();
    cardNum = element.atomNum.toInt();
    drawCard();
}

void assignValue(int index, String value) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The assignValue function takes in no paramaters.

It does not return any parameters.

This function is responsible for simply assigning the values read in from Serial
to the element struct.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // Assigning each value to the element struct
    switch (index) {
        case 1: element.atomNum = value;
        case 2: element.name = value;
        case 3: element.symbol = value;
        case 4: element.mass = value;
        case 5: element.neutrons = value;
        case 6: element.protons = value;
        case 7: element.electrons = value;
        case 8: element.period = value;
        case 9: element.group = value;
        case 10: element.phase = value;
        case 11: element.radioactive = value;
        case 12: element.natural = value;
        case 13: element.type = value;
        case 14: element.atomRadius = value;
        case 15: element.electroNeg = value;
        case 16: element.firstIon = value;
        case 17: element.density = value;
        case 18: element.mp = value;
        case 19: element.bp = value;
        case 20: element.numIsotopes = value;
        case 21: element.founder = value;
        case 22: element.year = value;
        case 23: element.heatCap = value;
        case 24: element.numShells = value;
        case 25: element.numValence = value;
    }
}



String read_word(uint32_t timeout) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The read_word function takes the parameters:
    timeout: the time required for a timeout

It returns the parameters:
    word: the value read in from serial

The point of this function is to read from serial up to a space
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    String word = "";
    char byte;
    uint32_t start = millis();  // start timer

    while (true) {
        if (Serial.available()) {
            byte = Serial.read();  // read in a byte
            if (byte != ' ') {
                word += byte;
            } else {
                return word;
            }
        } else if (millis() - start > timeout) {  // check timeout
            return "";
        }
    }
}


String read_value(uint32_t timeout) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The read_value function takes the parameters:
    timeout: the time required for a timeout

It returns the parameters:
    word: the value read in from serial

The point of this function is to read from serial up to a new line character
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    String word = "";
    char byte;
    uint32_t start = millis();  // start timer

    while (true) {
        if (Serial.available()) {
            byte = Serial.read();  // read in a byte
            if (byte != '\n') {
                word += byte;
            } else {
                return word;
            }
        } else if (millis() - start > timeout) {  // check timeout
            return "";
        }
    }
}


bool checkTimeout(bool timeout, uint32_t time, uint32_t startTime) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The checkTimeout function takes the parameters.
    timeout: the timeout boolean
    time: the time for timeout
    startTime: the starting time

It returns the parameters:
  timeout: the timeout boolean

The point of this function is to determine if a timeout occured since the
given startTime. If so timeout is set to true and returned.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    while (!Serial.available()) {
        if ((millis()-startTime) >= time) {  // check the timeout
            timeout = true;
            break;
        }
    }
    return timeout;
}

void searchRequest() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The searchRequest function takes in no paramaters.

It does not return any parameters.

This function is responsible for sending the search character to the Arduino
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    Serial.println("S");
}


void sendRequest() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The sendRequest function takes in no parameters.

It returns no parameters.

The point of this function is to send the request character, along with the
atomic number (card number) through serial
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    Serial.print("R ");
    Serial.print((String)cardNum);
    Serial.println(" ");
}


void sendAck() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The sendAck function takes no parameters.

It returns no parameters.

The point of this function is to send an acknowledgement to the server.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // send the A character followed by a newline
    Serial.println("A");
}


int getSearchResults(String arr[][2]) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The getSearchResults function takes in the paramaters:
    arr: a 2d array holding the top 4 search results

It returns the parameters:
    n : number of elements that have been generated and read in 

This function is responsible for getting the top 4 search results generated
on the server side and storing them in a 2d array to be accessed later in the
program
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // Gets the resulst from the server about the search
    String n;
    uint32_t start = millis();
    while (true) {
        // Check if timeout
        if (millis() - start > 5000) {
            failScreen();  // display a fail message
            return -1;  // timeout
        }
        bool failed = false;
        String n_char = read_word(100);  // read in n
        if (n_char == "N") {
            sendAck();  // send ack
            n = read_value(100);  // read in number of elements to read
            for (int i = 0; i < n.toInt(); i++) {
                String p = read_word(100);  // read in P character
                if (p == "P") {
                    sendAck();  // send ack
                    arr[i][0] = read_word(100);  // read atomNum
                    arr[i][1] = read_value(100);  // read name
                } else {
                    failed = true;  // failed flag = true
                }
            }
            if (failed) {
                continue;  // retry process
            } else {
                break;  // success, break out of loop
            }
        } else {
            Serial.flush();
            continue;  // retry request
        }
    }
    return n.toInt();
}


void waitingScreen() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The waitingScreen function takes in no paramaters.

It does not return any parameters.

This function is responsible for displaying a waiting screen and instructs the
user to type their search string in the terminal window.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    blankCard();
    tft.fillRect(20, 20, displayconsts::tft_width - sidebar - 40,
                 displayconsts::tft_height - 40, ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(45, 80);
    tft.print("Please enter your");
    tft.setCursor(40, 120);
    tft.print("element of choice");
    tft.setCursor(55, 160);
    tft.print("in the terminal");
}


void noMatchScreen() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The noMatchScreen function takes in no paramaters.

It does not return any parameters.

This function is responsible for displaying a message indicating that there were
no matches found.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    blankCard();
    tft.fillRect(20, 50, displayconsts::tft_width - sidebar - 40,
                 displayconsts::tft_height - 100, ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(50, 120);
    tft.print("No match found");
    delay(1500);
}

void failScreen() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The failScreen function takes in no paramaters.

It does not return any parameters.

This function is responsible for displaying a fail message in the case when the
search times out.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    blankCard();
    tft.fillRect(20, 50, displayconsts::tft_width - sidebar - 40,
                 displayconsts::tft_height - 100, ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(60, 120);
    tft.print("Search Failed");
    delay(1500);
}

int searchScreen(String arr[][2]) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The searchScreen function takes in the paramaters:
    arr: a 2D array to hold the top 4 results generated

It returns the parameters:
    cardNum: the cardNum that the user has picked

This function is responsible for taking care of all the initial duties of
correctly setting up the text and formatting it accordingly. The test used in
this exercise is all in one colour and size.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    blankCard();  // reset card

    /*Search box*/
    tft.fillRect(20, 20, displayconsts::tft_width - sidebar - 40, 30,
                 ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(45, 40);
    tft.print("Matched Elements");

    int startingPoints[4][2] = {{10, 65}, {10, 150}, {130, 65}, {130, 150}};
    int border = 2;
    int width = 100;
    int height = 70;

    /*draw boxes*/
    for (int i = 0; i < 4; i++) {
        tft.fillRect(startingPoints[i][0], startingPoints[i][1], width,
                     height, ILI9341_BLACK);
        tft.fillRect(startingPoints[i][0] + border, startingPoints[i][1]
                     + border, width - 2*border, height -
                     2*border, ILI9341_WHITE);
    }

    /*label boxes with search results*/
    int labelPoints[4][2] = {{15, 105}, {15, 190}, {140, 105}, {140, 190}};
    tft.setTextColor(ILI9341_BLACK);
    for (int i = 0; i < 4; i++) {
        tft.setCursor(labelPoints[i][0], labelPoints[i][1]);
        tft.print(arr[i][1]);
    }
    int selected;
    while (true) {
        /* MAKE THIS ITS OWN FUNCTION*/
        TSPoint touch = ts.getPoint();
        if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
            continue;
        }
        int16_t touched_x = map(touch.y, TS_MINY, TS_MAXY,
                             displayconsts::tft_width, 0);
        int16_t touched_y = map(touch.x, TS_MINX, TS_MAXX, 0,
                             displayconsts::tft_height);
        if (touched_x < startingPoints[0][0] + width &&
            touched_x > startingPoints[0][0]) {
            if (touched_y < startingPoints[0][1] + height &&
                touched_y > startingPoints[0][1]) {
                // top left box
                selected = arr[0][0].toInt();
            } else if (touched_y < startingPoints[1][1] + height
                       && touched_y > startingPoints[1][1]) {
                // bottom left box
                selected =  arr[1][0].toInt();
            }
        } else if (touched_x > startingPoints[2][0] &&
                   touched_x < startingPoints[2][0] + width) {
            if (touched_y < startingPoints[2][1] + height &&
                touched_y > startingPoints[2][1]) {
                // top right box
                selected =  arr[2][0].toInt();
            } else if (touched_y < startingPoints[3][1] + height &&
                       touched_y > startingPoints[3][1])  {
                // bottom right box
                selected = arr[3][0].toInt();
            }
        }
        if (selected >= 1 && selected <= 118) {
            return selected;  // Check if good selection
        } else {
            return cardNum;  // If failed return current cardNum
        }
    }
}


void clientCom() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The clientCom function takes in no parameters.

It returns no parameters.

The point of this function is to process the communication between the Arduino
and the server. It reads in the element and saves it to the element struct
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    while (true) {
        Serial.flush();
        bool timeout = false;
        uint32_t startTime = millis();
        if (!search) {
            sendRequest();  // send the request
            timeout = checkTimeout(timeout, 1000, startTime);
        } else {
            waitingScreen();  // display wait screen
            searchRequest();  // send search request

            /*Read in search predictions*/
            timeout = checkTimeout(timeout, 100000, startTime);
            String arr[4][2];  // array to hold 4 predictions

            int n = getSearchResults(arr);
            if (n > 1) {
                cardNum = searchScreen(arr);
            } else if (n == 1) {
                cardNum = arr[0][0].toInt();
            } else {
                if (n != -1) {
                    noMatchScreen();  // no matches found
                }
            }
            search = false;
            continue;
        }

        if (Serial.available() && !timeout) {
            for (int count = 1; count <= 25; count ++) {
                String letter = read_word(100);  // read in the letter C
                if (letter == "C") {
                    Serial.read();  // read in space
                    String num = read_word(100);  // read in the num
                    if (num.toInt() != count) {
                        timeout = true;  // timeout
                    } else {
                        Serial.read();  // read in space
                        String value = read_value(100);  // read in value
                        sendAck();  // send ack
                        assignValue(num.toInt(), value);  // assign the values
                    }
                } else {
                    // send request again with the same point
                    timeout = true;
                }
            }
        }
        if (timeout) {
            read_value(10);  // reading in any junk
            continue;  // retry request
        } else {
            break;  // process finished
        }
    }
}


int main() {
    setup();  // setup program
    welcomeScreen();  // display welcome screen
    nextCard();  // Get initial card
    while (true) {
        getTouch();  // keep reading Arduikno input devices
    }
    return 0;
}
