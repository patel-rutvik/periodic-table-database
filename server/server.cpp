#include "types.h"


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
            /*
            if (token == "V") {
                getline(file, token, delim);
                ID = stoi(token);
                getline(file, token, delim);
                coord = stold(token) * 100000;
                coord = static_cast<ll>(coord);
                p.lat = coord;  // saving the coordinate to the points
                getline(file, token);
                coord = stold(token) * 100000;
                coord = static_cast<ll>(coord);
                p.lon = coord;  // saving the coordinate to the points
                points[ID] = p;  // insert into the map
            } else if (token == "E") {
                getline(file, token, delim);
                int id1 = stoi(token);
                getline(file, token, delim);
                int id2 = stoi(token);
                getline(file, token);
                string name = token;
                p1 = points[id1];  // get respective point
                p2 = points[id2];  // get respective point
                // calc man dist, pass in saved points
                ll weight = manhattan(p1, p2);  // calculate distance
                graph.addEdge(id1, id2, weight);  // add the weighted edge
            }
            */
        }
    } else {
        /*Error message in case the file is not found. */
        cout << "ERROR. Unable to open the file " << filename << "." << endl;
    }
    file.close();  // closing the file
}

int main() {
    unordered_set<Element, elementHash> elements;
    readFile("table-data.txt", elements);


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

    return 0;
}