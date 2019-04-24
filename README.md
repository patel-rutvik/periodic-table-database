# Periodic Table Database
## Summary
A database for elements in the periodic table where the user can browse through all the elements, with different UI layouts available to use. This project also has predictive search, using the [Trie](https://en.wikipedia.org/wiki/Trie "Trie") data structure. The user is able to search for a desired element, and if partially correct, the program will print out suggestions that the user can then select from.

## Running Instructions
### Hardware Components:
- Arduino Mega 2560 Board
- Banana plug wires
- 2 button components
- 2 10k resistors
- Adafruit TFT display
### Wiring Instructions
| Display Pins | Arduino Pins |
| ------------ | ------------ |
| 1 GND | BB GND bus|
| 2 Vin | BB Vcc (positive) bus |
| 4 CLK | Pin 52 |
| 5 MISO | Pin 50 |
| 6 MOSI | Pin 51 |
| 7 CS | Pin 10 |
| 8 D/C | Pin 9|
| 19 CCS | Pin 6 |

| Button Pins | Arduino Pins |
| ------------ | ------------ |
| Left button | Pin 2 |
| Right button | Pin 3 |



In order to correctly run the program, you must first navigate to the source directory with the project files. Navigate to the **client** subdirectory, then use the command:
```bash
make upload
```
After running the command above, navigate to the **server** subdirectory and use the command:
```bash
make && ./server
```
The server will be brought online and the program will be running.
## How to Use
Once the program is uploaded to the Arduino and the server is brought online, the following user actions are permitted:
- The user can use the left/right buttons to *scroll* through the cards
- The user can *tap* the screen to **flip** the card and see more information
- The user can click the **L** button to cycle through different layouts, each with different verbosity of information
- The user can click the **search** button to initiate a search
### Search Protocol
Once the user clicks the search button, use the following steps to search for an element:
1. Type in your search string in the terminal when prompted
2. Press enter once your search string is complete
3. If a partial search is entered, select one of the suggestions from the screen
4. The program will then jump to the selected card


## Miscellaneous
### Makefile (server) Targets
- make (**server**) : builds the project and generates the server executable
- make **serialport.o** : compiles serialport.cpp into its respective object (.o) file
- make **server.o** : compiles server.cpp into its respective object (.o) file
- make **clean** : removes the .o and executable files from the source directory
- make **help** : prints out further information about the makefile targets available to use
