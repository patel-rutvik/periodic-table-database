/*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Final Project: Periodic Table Database
Names: Rutvik Patel, Kaden Dreger
ID: 1530012, 1528632
CCID: rutvik, kaden
CMPUT 275 Winter 2018

This program demonstrates a server-side program. It handles all
computations and relays it to the server. It also communicates with the client
side and sends the element in question.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
#include "types.h"

struct TrieNode *root = getNode();  // establishing the trie root nodes


// This is a modified implementation of split from:
// https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
vector<string> split(string str, char delim) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The split function takes the parameters:
    str: the string to be split
    delim: the delimiter for splitting

It returns the parameters:
    vector<string>: a vector containing the string split up

The point of this function is to split the given string by the given delimiter
abd return it as a vector.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    vector<string> temp;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        temp.push_back(token);
    }
    return temp;
}


void readFile(string filename, unordered_set<Element, elementHash>& table) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The readFile function takes the parameters:
    filename: the name of the file to be read
    table: the hash table to store the elements

It returns no parameters.

The point of this function is to read in the CSV file and assign the values
into the table.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /* method to open and read from files found from:
    cplusplus.com/doc/tutorial/files/             */
    ifstream file;  // file object
    string token;
    char delim = ',';  // the delimeter of choice
    file.open(filename);  // opening the file
    if (file.is_open()) {  // if the file is open...
        Element temp;
        for (int i = 0; i < 28; i++) {
            if (i < 27) {
                getline(file, token, delim);  // reading in first label line
            } else {
                getline(file, token);
            }
        }
        while (getline(file, token, delim)) {
            temp.atomNum = token;
            getline(file, token, delim);
            string tempString = token;
            tempString[0] = tolower(tempString[0]);
            temp.name = tempString;
            insert(root, tempString);
            getline(file, token, delim);
            temp.symbol = token;
            getline(file, token, delim);
            temp.mass = token;
            getline(file, token, delim);
            temp.neutrons = token;
            getline(file, token, delim);
            temp.protons = token;
            getline(file, token, delim);
            temp.electrons = token;
            getline(file, token, delim);
            temp.period = token;
            getline(file, token, delim);
            temp.group = token;
            getline(file, token, delim);
            temp.phase = token;
            getline(file, token, delim);
            temp.radioactive = token;
            getline(file, token, delim);
            temp.natural = token;
            getline(file, token, delim);  // metal  - not used           
            getline(file, token, delim);  // nonmetal - not used
            getline(file, token, delim);  // metalloid - not used
            getline(file, token, delim);
            temp.type = token;
            getline(file, token, delim);
            temp.atomRadius = token;
            getline(file, token, delim);
            temp.electroNeg = token;
            getline(file, token, delim);
            temp.firstIon = token;
            getline(file, token, delim);
            temp.density = token;
            getline(file, token, delim);
            temp.mp = token;
            getline(file, token, delim);
            temp.bp = token;
            getline(file, token, delim);
            temp.numIsotopes = token;
            getline(file, token, delim);
            temp.founder = token;
            getline(file, token, delim);
            temp.year = token;
            getline(file, token, delim);
            temp.heatCap = token;
            getline(file, token, delim);
            temp.numShells = token;
            getline(file, token);
            temp.numValence = token;
            table.insert(temp);  // insert the constructed element in the table
        }
    } else {
        /*Error message in case the file is not found. */
        cout << "ERROR. Unable to open the file " << filename << "." << endl;
    }
    file.close();  // closing the file
}


Element findElement( unordered_set<Element, elementHash>& elements, string num) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The findElement function takes the parameters:
    elements: the hash table holding all the elements
    num: the atomic number to fine

It returns the parameters:
    i: the element that matches

The point of this function is to find an element in the hash table based on the
atomic number passed in.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    for (auto i : elements) {  // for i in elements...
        if (i.atomNum == num) {  // if the atomic number matches num
            return i;  // return that element
        }
    }
}

Element findName( unordered_set<Element, elementHash>& elements, string name) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The findName function takes the parameters:
    elements: the hash table holding all the elements
    name: the element name to be searched

It returns the parameters:
    i: the element that matches

The point of this function is to find an element in the has table based on the
name passed in.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    for (auto i : elements) {  // for i in elements...
        if (i.name == name) {  // if the element name matches the name
            return i;  // return that element
        }
    }
}


string getProperties(Element requestElement, int index) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The getProperties function takes the parameters:
    requestElement: the element in question
    index: the number corresponding to the method of the struct

It returns the property queried.

The point of this function is to return a certain property inside the element
structure created.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    switch (index) {
        case 1: return requestElement.atomNum;
        case 2: return requestElement.name;
        case 3: return requestElement.symbol;
        case 4: return requestElement.mass;
        case 5: return requestElement.neutrons;
        case 6: return requestElement.protons;
        case 7: return requestElement.electrons;
        case 8: return requestElement.period;
        case 9: return requestElement.group;
        case 10: return requestElement.phase;
        case 11: return requestElement.radioactive;
        case 12: return requestElement.natural;
        case 13: return requestElement.type;
        case 14: return requestElement.atomRadius;
        case 15: return requestElement.electroNeg;
        case 16: return requestElement.firstIon;
        case 17: return requestElement.density;
        case 18: return requestElement.mp;
        case 19: return requestElement.bp;
        case 20: return requestElement.numIsotopes;
        case 21: return requestElement.founder;
        case 22: return requestElement.year;
        case 23: return requestElement.heatCap;
        case 24: return requestElement.numShells;
        case 25: return requestElement.numValence;
    }
}


bool sendElement(unordered_set<Element, elementHash>& elements,
                 Element& requestElement) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The sendElement function takes the parameters:
    elements: the hash table containing all the elements
    requestElement: the element to be sent

It returns the parameters:
    bool: This indicates if there was a timeout

The point of this function is to send the properties of the requested element
to the arduino via the serial port.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    string ack, output;
    ack = "A\n";
    for (int i = 1; i <= 25; i++) {
        if (ack == "A\n") {
            /*print out the waypoint coordinates*/
            output = getProperties(requestElement, i);
            port.writeline("C ");
            port.writeline(to_string(i));
            port.writeline(" ");
            port.writeline(output);
            port.writeline("\n");
            ack = port.readline(100);
        } else {
            // timeout...
            cout << "timeout.." << endl << endl;
            return true;
        }
    }
    cout << "sent last element" << endl;
    // receive acknowledgement
    port.readline(100);
    // indicating end of request
    sendFailed = false;
    return false;  // no timeout
}


void processRequest(unordered_set<Element, elementHash>& elements) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The processRequest function takes the parameters:
    elements: the hash table of elements

It returns no parameters.

The point of this function is to process the communication with the Arduino
by routing to the correct state based on the string processed from serial.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    bool timeout = false;  // initialize timeout to be false
    string temp = port.readline(0);  // wait for the state-determining string
    if (temp[0] == 'R') {  // request state
        vector<string> request = split(temp, ' ');  // split request string
        cout << "Getting request..." << endl;
        cout << endl;
        string atomNum = request[1];
        // Find the element in the table and return the struct.
        Element requestElement = findElement(elements, atomNum);

        timeout = sendElement(elements, requestElement);
        request.clear();  // clearing the vector
        if (!timeout) {
            cout << "sent element to client" << endl << endl;
        } else {
            cout << "failed to send element" << endl << endl;
        }
    } else if (temp[0] == 'S') {  // search state
        if (!sendFailed) {  // if no timeout...
            cout << "Enter element name here: ";            
            cin >> nameRequest;  // get name request from terminal
            nameRequest[0] = tolower(nameRequest[0]);  // Making lowercase
        }
        getSearchResults(root, nameRequest, elements);  // get the search preds.
        sendPredictions();  // send the generated predictions
        nameRequest.clear();  // clearing string for next search
        if (timeout) {
            sendFailed = true;  // sending process failed
        } else {
            sendFailed = false;  // sent successfully
        }
    }
}


int main() {
    cout << "starting program" << endl;
    unordered_set<Element, elementHash> elements;  // hash table of elements
    readFile("table-data.txt", elements);  // read in the CSV
    while (true) {
        processRequest(elements);  // wait for the next request from client
    }
    return 0;
}
