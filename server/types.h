#ifndef TYPES_H
#define TYPES_H

#include "serialport.h"
#include "element.h"
#include <fstream>  // for file
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <utility>  // for pair
#include <sstream>
#include <algorithm>
#include <string.h>
#include "trie.h"

using namespace std;

bool sendFailed = false;
string nameRequest = "";
SerialPort port("/dev/ttyACM0");



vector<string> split(string str, char delim);
void readFile(string filename, unordered_set<Element, elementHash>& table);
Element findElement( unordered_set<Element, elementHash>& elements, string num);
Element findName( unordered_set<Element, elementHash>& elements, string name);
string getProperties(Element requestElement, int index);
bool sendElement(unordered_set<Element, elementHash>& elements, Element& requestElement);
void processRequest(unordered_set<Element, elementHash>& elements);



void sendPredictions() {
    string ack, output;
    int n = predictions.size();  // finding number of predictions generated
    clock_t start = clock();
    while (true) {
        if ((clock() - start)/CLOCKS_PER_SEC > 5) {
            break;
        }
        bool failed = false;
        cout << endl;
        cout << "sending results to Arduino..." << endl;
        
        /*Send number of predictions*/
        port.writeline("N");
        port.writeline(" ");
        port.writeline(to_string(n));
        port.writeline("\n");
        cout << "N " << n << endl;
        
        ack = port.readline(100);  // receive ack
        if (ack != "A\n") {
            cout << "Ack for N not received, retrying com..." << endl;
            continue;
        } else {
            cout << "ack for N successfully received" << endl;
        }

        /*Sending all the predicted elements*/
        for (int i = 0; i < n; i++) {
            if (ack == "A\n") {
                if (i > 0) {
                    cout << "ack received" << endl;
                }
                port.writeline("P");  //prediction character
                port.writeline(" ");
                pair<string, string> temp = predictions[i];
                port.writeline(temp.first);  // sending atomic num
                port.writeline(" ");
                port.writeline(temp.second);  // sending name
                port.writeline("\n");  // sending end line
                cout << "P " << temp.first << " " << temp.second << endl;
                
                //predictions.pop();  // removing the element we just sent
                ack = port.readline(100);  // receive ack
                if (ack != "A\n") {
                    failed = true;
                }
            } else {
                if (i > 0) {
                    cout << "Ack for prediction not received" << endl;
                }
                failed = true;
            }

        }

        /*empty stack*/
        if (!failed) {
            cout << "Sent all predictions successfully..." << endl;
            predictions.clear();
            break;
        }
    }
}

#endif /*TYPES_H*/
