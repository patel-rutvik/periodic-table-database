#ifndef TYPES_H
#define TYPES_H


#include <fstream>  // for file
#include <iostream>
#include <string>

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
    string metal;
    string nonmetal;
    string metalloid;
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
};



#endif /*TYPES_H*/