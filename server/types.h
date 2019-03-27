#ifndef TYPES_H
#define TYPES_H


#include <fstream>  // for file
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <sstream>

using namespace std;

struct Element {
    string atomNum;
    string name;
    string symbol;
    string mass;
    string neutrons;
    string protons;
    string electrons;
    string period;
    string group;
    string phase;
    string radioactive;
    string natural;
    string type;
    string atomRadius;
    string electroNeg;
    string firstIon;
    string density;
    string mp;
    string bp;
    string numIsotopes;
    string founder;
    string year;
    string heatCap;
    string numShells;
    string numValence;

    /*Operator overloading and hashing method is a modified implementation from:
    https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom
    -class-type-as-the-key?noredirect=1&lq=1 
    From user: jogojapan @ June 10, 2013 */
    bool operator==(const Element &thing) const {
        // overloading the == operator
        return (atomNum == thing.atomNum);
    }
};

struct elementHash {
    // This is the custom hash to accomodate for the unique ID's to hash in
    // the StudentRecord struct
    size_t operator()(Element const& element) const {
        // hash the id, and return
        return hash<string>()(element.atomNum);
    }
};


#endif /*TYPES_H*/