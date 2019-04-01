#include "util.h"
#include "lcd_image.h"
#include "types.h"


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
Element element;
uint8_t left_pushed;
uint8_t right_pushed;

void drawCard();
void clientCom();
void minimalCard();
void classicCard();
void compactCard();
void nextCard();
void searchRequest();


void setup() {
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
    tft.setCursor(displayconsts::tft_width/3, displayconsts::tft_height/4);
    textInit();
    tft.print("Welcome!");
    tft.setCursor(displayconsts::tft_width/6, displayconsts::tft_height/4 + 50);
    tft.print("Periodic Table Database");
    tft.setCursor(displayconsts::tft_width/6 + 10, displayconsts::tft_height/2 + 40);
    tft.print("By: Rutvik and Kaden");
    delay(1000);
    tft.fillScreen(ILI9341_BLACK);
}


void drawButtons() {
    tft.setTextSize(1);
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


void getTouch() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The getTouch function takes no paramaters:

It does not return any parameters.

The point of this function is to...
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    left_pushed = (digitalRead(clientpins::left_pin) == HIGH);
    right_pushed = (digitalRead(clientpins::right_pin) == HIGH);
    delay(100);
    if (left_pushed && cardNum > 1) {
        cardNum--;
        nextCard();
        return;
    } else if (right_pushed && cardNum < 118) {
        cardNum++;
        nextCard();
        return;
    } else if (right_pushed && cardNum == 118) {
        cardNum = 1;
        nextCard();
        return;
    } else if (left_pushed && cardNum == 1) {
        cardNum = 118;
        nextCard();
        return;
    }

    TSPoint touch = ts.getPoint();
    if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
        return;
    }
    // mapping to the screen, same implementation as we did in class
    int16_t touched_x = map(touch.y, TS_MINY, TS_MAXY, displayconsts::tft_width, 0);
    int16_t touched_y = map(touch.x, TS_MINX, TS_MAXX, 0, displayconsts::tft_height);
    if (touched_x < displayconsts::tft_width - 48) {
        //flip card here...
        if (!flipped) {
            flipped = true;
        } else {
            flipped = false;
        }
        drawCard();
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
            drawCard();
        } else if (touched_y >= displayconsts::tft_height/2 + 2) {
            search = true;
            nextCard();
            search = false;
        }
        delay(100);
    }
}


void blankCard() {
    // blank white card
    //tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, ILI9341_WHITE);
    if (element.type == "Nonmetal") {  // Oxygen
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(128, 235, 128));
    } else if (element.type == "Transition Metal") {  // Gold
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(255, 120, 120));
    } else if(element.type == "Noble Gas") {  // Helium
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(173, 216, 250));
    } else if (element.type == "Alkali Metal") {  // Lithium
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(255, 180, 98));
    } else if(element.type == "Alkaline Earth Metal") {  // Calcium
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(0, 140, 140));
    } else if (element.type == "Halogen") {  // Fluorine
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(50, 255, 241));
    } else if (element.type == "Lanthanide") {
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(255, 255, 255));
    } else if(element.type == "Metal") {  // Tin
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(250, 250, 100));
    } else if (element.type == "Metalloid") { // Boron
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(197, 139, 231));
    } else if (element.type == "Actinide") {
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(255, 255, 255));
    } else if (element.type == "Transactinide") {
        tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(255, 255, 255));
    }

    /* draw buttons */
    drawButtons();
}


void backCard() {
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0,15);
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
    blankCard();
    if (!flipped) {
        /*
        String temp, other;
        temp = element.name;
        other = temp.toUpperCase();
        element.name = other;
        */
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(0, 25);
        tft.print(element.atomNum);
        
        tft.setCursor(0, 90);
        tft.setTextSize(4);
        tft.print(element.symbol);
        tft.setCursor(0, 130);
        //tft.setCursor((displayconsts::tft_width - sidebar)/3, displayconsts::tft_height/5);
        tft.setTextSize(2);
        tft.print(element.name);
        
        //tft.setCursor((displayconsts::tft_width - sidebar)/3 + 5, displayconsts::tft_height/3);
        //tft.print(element.mass);
        //tft.setCursor((displayconsts::tft_width - sidebar)/3, displayconsts::tft_height - 10);
        //tft.print("minimal layout");
    } else {
        backCard();
    }
}


void classicCard() {
    blankCard();
    if (!flipped) {
        /*
        String temp, other;
        temp = element.name;
        other = temp.toUpperCase();
        element.name = other;
        */
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
        //tft.setCursor((displayconsts::tft_width - sidebar)/3, displayconsts::tft_height/5);
        tft.setTextSize(2);
        tft.print(element.name);
        tft.setTextSize(1);
        tft.setCursor(0, 160);
        tft.print(element.mass);
    } else {
        backCard();
    }
}


void compactCard() {
    blankCard();
    if (!flipped) {
        /*
        String temp, other;
        temp = element.name;
        other = temp.toUpperCase();
        element.name = other;
        */
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
        //tft.setCursor((displayconsts::tft_width - sidebar)/3, displayconsts::tft_height/5);
        tft.setTextSize(1);
        tft.print(element.name);
        
        tft.setCursor(0, 110);
        tft.print(element.mass);
    } else {
        backCard();
    }    
}


void drawCard() {
    if (currentLayout == 1) {
        minimalCard();
    } else if (currentLayout == 2) {
        classicCard();
    } else if (currentLayout == 3) {
        compactCard();
    }
}

void nextCard() {
    clientCom();
    cardNum = element.atomNum.toInt();
    drawCard();
}

void assignValue(int index, String value) {
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
The readWaypoint function takes the parameters:
    timeout: the time required for a timeout

It returns the parameters:
    word: the waypoint read in from serial mon

The point of this function is to read in the waypoint and determine if a timeout
occured.
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
The readWaypoint function takes the parameters:
    timeout: the time required for a timeout

It returns the parameters:
    word: the waypoint read in from serial mon

The point of this function is to read in the waypoint and determine if a timeout
occured.
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
The sendRequest function takes the parameters:
    start: the starting point
    end: the ending point

It returns no parameters.

The point of this function is to send the starting and ending points from the
Arduino to the server.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    Serial.flush();
    Serial.println("S");
    Serial.flush();
}


void sendRequest() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The sendRequest function takes the parameters:
    start: the starting point
    end: the ending point

It returns no parameters.

The point of this function is to send the starting and ending points from the
Arduino to the server.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    Serial.flush();
    Serial.print("R ");
    Serial.print((String)cardNum);
    Serial.println(" ");
    Serial.flush();
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

void getSearchResults(String arr[][2]) {
    while (true) {
        uint32_t startTime = millis();
        String n_char = read_word(100);
        //timeout = checkTimeout(timeout, 100, startTime);

        if (n_char == "N") {
            //sendAck();
            sendAck();
            //tft.fillRect(0, 0, displayconsts::tft_width - sidebar, displayconsts::tft_height, tft.color565(255, 0, 0));
            //delay(3000);
            //Serial.read();  // read space
            String n = read_value(100);
            
            //Serial.read();
            //ptr = arr;
            for (int i = 0; i < n.toInt(); i++) {
                //sendAck();
                String p = read_word(100);
                //Serial.read();
                if (p == "P") {
                    sendAck();
                    //sendAck();
                    arr[i][0] = read_word(100);
                    //Serial.read();
                    arr[i][1] = read_value(100);
                    
                } else {
                    //timeout = false;
                    continue; // retry request?
                    //sendAck();
                    
                }
            }
            break;
            //read_value(100);
            //sendAck();
            //ptr = arr;
        } else {
            Serial.flush();
            continue;  // retry request???
            //timeout = false;
            
        }
    }
}
void waitingScreen() {
    blankCard();
    tft.fillRect(20, 20, displayconsts::tft_width - sidebar - 40, displayconsts::tft_height - 40, ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(45, 80);
    tft.print("Please enter your");
    tft.setCursor(40,120);
    tft.print("element of choice");
    tft.setCursor(55, 160);
    tft.print("in the terminal");

}




int searchScreen(String arr[][2]) {
    blankCard();  // reset card
    
    /*Search box*/
    tft.fillRect(20, 20, displayconsts::tft_width - sidebar - 40, 30, ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(45, 40);
    tft.print("Matched Elements");

    int startingPoints[4][2] = {{10,65}, {10, 150}, {130, 65}, {130, 150}};
    int border = 2;
    int width = 100;
    int height = 70;

    /*draw boxes*/
    for (int i = 0; i < 4; i++) {
        tft.fillRect(startingPoints[i][0], startingPoints[i][1], width, height, ILI9341_BLACK);
        tft.fillRect(startingPoints[i][0] + border, startingPoints[i][1] + border, width - 2*border, height - 2*border, ILI9341_WHITE);
    }

    /*label boxes with search results*/
    //String searchResults[4] = {"hydrogen", "plutonium" ,"oxygen" , "iron"};

    int labelPoints[4][2] = {{15, 105}, {15, 190}, {140, 105}, {140, 190}};
    tft.setTextColor(ILI9341_BLACK);
    for (int i = 0; i < 4; i++) {
        tft.setCursor(labelPoints[i][0], labelPoints[i][1]);
        tft.print(arr[i][1]);
    }

    while (true) {
        TSPoint touch = ts.getPoint();
        if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
            continue;
        }
        int16_t touched_x = map(touch.y, TS_MINY, TS_MAXY, displayconsts::tft_width, 0);
        int16_t touched_y = map(touch.x, TS_MINX, TS_MAXX, 0, displayconsts::tft_height);
        if (touched_x < startingPoints[0][0] + width) {
            if (touched_y < startingPoints[0][1] + height && touched_y > startingPoints[0][1]) {
                // top left box
                return arr[0][0].toInt();
                
            } else if (touched_y < startingPoints[2][1] + height && touched_y > startingPoints[2][1]) {
                // bottom left box
                return arr[1][0].toInt();
            }    
        } else if (touched_x > startingPoints[1][0]) {
            if (touched_y < startingPoints[1][1] + height && touched_y > startingPoints[1][1]) {
                // top right box
                return arr[2][0].toInt();
            } else if (touched_y < startingPoints[3][1] + height && touched_y > startingPoints[3][1])  {
                // bottom right box
                return arr[3][0].toInt();
            }
        }
    }

}


void clientCom() {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The clientCom function takes the parameters.
    start: the starting point
    end: the end point

It returns no parameters.

The point of this function is to process the communication between the Arduino
and the server. It reads in the waypoints and stores them in shared.waypoints.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    while (true) {
        Serial.flush();
        bool timeout = false;
        uint32_t startTime = millis();
        if (!search) {
            sendRequest();  // send the request
            timeout = checkTimeout(timeout, 1000, startTime);
        } else {
            waitingScreen();
            searchRequest();  // send search request

            /*Read in search predictions*/
            timeout = checkTimeout(timeout, 10000, startTime);
            String arr[4][2];  // array to hold 4 predictions
            //timeout = false;
            //while(!Serial.available()) {}
            
            getSearchResults(arr);

            cardNum = searchScreen(arr);
            //searchScreen();
            //delay(10000);
            search = false;
            continue;
            
        }

        if (Serial.available() && !timeout) {
            for (int count = 1; count <= 25; count ++) {
                String letter = read_word(100); // read in the letter
                if (letter == "C") {
                    Serial.read();  // read in space
                    // reading in the number of waypoints
                    String num = read_word(100);
                    if (num.toInt() != count) {
                        timeout = true;
                    } else {
                        Serial.read();
                        String value = read_value(100);
                        sendAck();
                        assignValue(num.toInt(), value);
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
            break;
        }
    }
}


int main() {
    setup();  // setup program
    welcomeScreen();  // display welcome screen
    nextCard();
    while (true) {
        getTouch();
    }
    return 0;
}