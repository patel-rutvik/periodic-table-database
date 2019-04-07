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
#ifndef TYPES_H
#define TYPES_H

// Struct that contains all the properties of an element.
struct Element {
    String atomNum;
    String name;
    String symbol;
    String mass;
    String neutrons;
    String protons;
    String electrons;
    String period;
    String group;
    String phase;
    String radioactive;
    String natural;
    String type;
    String atomRadius;
    String electroNeg;
    String firstIon;
    String density;
    String mp;
    String bp;
    String numIsotopes;
    String founder;
    String year;
    String heatCap;
    String numShells;
    String numValence;
} element;

#endif /*TYPES_H*/