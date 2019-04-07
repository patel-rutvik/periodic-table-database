/*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Final Project: Periodic Table Database
Names: Rutvik Patel, Kaden Dreger
ID: 1530012, 1528632
CCID: rutvik, kaden
CMPUT 275 Winter 2018

This file holds the Element struct for each element read in from the CSV.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
#ifndef ELEMENT_H
#define ELEMENT_H

// Struct that contains all the properties of an element.
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
    // This is the custom hash to accomodate for the unique atomic numbers in
    // the Element struct
    size_t operator()(Element const& element) const {
        // hash the atomNum, and return
        return hash<string>()(element.atomNum);
    }
};

#endif /*ELEMENT_H*/