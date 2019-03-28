#include "types.h"
#include "serialport.h"


SerialPort port("/dev/ttyACM0");


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
            //int index = 0;
            //getline(file, token, delim);
            temp.atomNum = token;
            //cout << "atomic number read in: " << token << endl;
            getline(file, token, delim);
            temp.name = token;
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
            getline(file, token, delim);  // metal            
            getline(file, token, delim);  // nonmetal
            getline(file, token, delim);  // metalloid
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

            table.insert(temp);
        }
    } else {
        /*Error message in case the file is not found. */
        cout << "ERROR. Unable to open the file " << filename << "." << endl;
    }
    file.close();  // closing the file
}


Element findElement(unordered_set<Element, elementHash>& elements, string num) {
    for (auto i : elements) {
        if (i.atomNum == num) {
            cout << "Found element" << endl;
            return i;
        }
    }
}


string printProperties(Element requestElement, int index) {
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


bool sendElement(unordered_set<Element, elementHash>& elements, Element& requestElement) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The printWaypoints function takes the parameters:
    tree     : the search tree with respective to the starting vertex
    startVert: starting vertex
    endVert  : end vertex
    p        : a mapping b/w vertex ID's and their respective coordinates

It does not return any parameters.

The point of this function is to print the number of waypoints, along with their
lat and lon values, enroute to the end vertex.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    string ack, output;
    for (int i = 1; i <= 25; i++) {
        // receive acknowledgement
        if (i == 1) {
            cout << endl;
            /*print out the waypoint coordinates*/
            output = printProperties(requestElement, i);
            cout << output << endl;
            cout << "C " << i << " " << output << endl;
            port.writeline("C ");
            port.writeline(to_string(i));
            port.writeline(" ");
            port.writeline(output);
            port.writeline("\n");
            continue;
        }
        do {
            ack = port.readline(1000);
        } while (ack == "");
        cout << "ack received!" << endl;
        if (ack == "A\n") {
            cout << endl;
            /*print out the waypoint coordinates*/
            output = printProperties(requestElement, i);
            cout << "C " << i << " " << output << endl;
            port.writeline("C ");
            port.writeline(to_string(i));
            port.writeline(" ");
            port.writeline(output);
            port.writeline("\n");
        } else {
            // timeout...
            cout << "timeout.." << endl << endl;
            return true;
        }
    }
    cout << "HELLO" << endl;
    // receive acknowledgement
    ack = port.readline(1000);
    if (ack == "") {
        return true;
    }
    if (ack[0] == 'A') {
        port.writeline("E\n");
    } else {
        return true;
    }
    // indicating end of request
    return false;  // no timeout
}


void processRequest(unordered_set<Element, elementHash>& elements) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The processRequest function takes the parameters:
    graph: the Digraph created from the csv file
    points: the points for each vertex

It returns no parameters.

The point of this function is to process the communication with the Arduino
by sending the number of waypoints and the waypoints themselves.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    bool timeout = false;
    string temp = port.readline(0);
    if (temp[0] == 'R') {
        vector<string> request = split(temp, ' ');  // find citation later...
        cout << "Getting request..." << endl;
        cout << endl;
        string atomNum = request[1];
        cout << atomNum << endl;
        // Find the element in the table and return the struct.
        Element requestElement = findElement(elements, atomNum);

            /* print out the waypoints enroute to the destination */
        timeout = sendElement(elements, requestElement);
        if (!timeout) {
            cout << "sent element to client" << endl << endl;
        } else {
            cout << "failed to send element" << endl << endl;
        }
    } else if (temp[0] == 'S') {
        // DO search stuff
        cout << "SEARCH" << endl;
    }

}


int main() {
    unordered_set<Element, elementHash> elements;
    readFile("table-data.txt", elements);
    /*
    //searching by name
    for (auto i : elements) {
        if (i.name == "Helium") {
            cout << i.atomNum << endl;
        }
    }


    // searching by atomic number
    for (auto i : elements) {
        if (i.atomNum == "3") {
            cout << i.numValence << endl;
        }
    }
    */
    while (true) {
        processRequest(elements);
    }

    return 0;
}