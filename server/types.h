#ifndef TYPES_H
#define TYPES_H

#include "serialport.h"
#include <fstream>  // for file
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <stack>
#include <utility>
#include <sstream>
#include <algorithm>
#include <string.h>

using namespace std;

stack<pair<string, string>> predictions;  // vector to store search predictions
SerialPort port("/dev/ttyACM0");

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
Element findName(unordered_set<Element, elementHash>& elements, string name);


//#include<bits/stdc++.h>
using namespace std;

// Alphabet size (# of symbols)
#define ALPHABET_SIZE (26)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];

    // isWordEnd is true if the node represents
    // end of a word
    bool isWordEnd;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = new TrieNode;
    pNode->isWordEnd = false;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->children[i] = NULL;
    }

    return pNode;
}

// If not present, inserts key into trie. If the
// key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, const string key)
{
    struct TrieNode *pCrawl = root;

    for (int level = 0; level < key.length(); level++)
    {
        int index = CHAR_TO_INDEX(key[level]);
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = getNode();
        }

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isWordEnd = true;
}

// Returns true if key presents in trie, else false
bool search(struct TrieNode *root, const string key)
{
    int length = key.length();
    struct TrieNode *pCrawl = root;
    for (int level = 0; level < length; level++)
    {
        int index = CHAR_TO_INDEX(key[level]);

        if (!pCrawl->children[index]) {
            return false;
        }

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isWordEnd);
}

// Returns 0 if current node has a child
// If all children are NULL, return 1.
bool isLastNode(struct TrieNode* root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i])
            return 0;
    return 1;
}

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
            if (ack == "A\n" && !predictions.empty()) {
                if (i > 0) {
                    cout << "ack received" << endl;
                }
                port.writeline("P");  //prediction character
                port.writeline(" ");
                pair<string, string> temp = predictions.top();
                port.writeline(temp.first);  // sending atomic num
                port.writeline(" ");
                port.writeline(temp.second);  // sending name
                port.writeline("\n");  // sending end line
                cout << "P " << temp.first << " " << temp.second << endl;
                
                predictions.pop();  // removing the element we just sent
                ack = port.readline(100);  // receive ack
            } else {
                if (i > 0) {
                    cout << "Ack for prediction not received" << endl;
                }
                failed = true;
            }

        }

        /*empty stack*/
        while (!predictions.empty()) {
            predictions.pop();
        }
        if (!failed) {
            cout << "Sent all predictions successfully..." << endl;
            break;
        }
    }
}

// Recursive function to print auto-suggestions for given
// node.
void suggestionsRec(struct TrieNode* root, string currPrefix,  unordered_set<Element, elementHash>& elements)
{
    // found a string in Trie with the given prefix
    if (root->isWordEnd)
    {
        cout << currPrefix << endl;

        Element element = findName(elements, currPrefix);
        pair<string, string> temp_pair = make_pair(element.atomNum, element.name);
        if (predictions.size() < 4) {
            predictions.push(temp_pair);  // adding pair to stack
        }
    }

    // All children struct node pointers are NULL
    if (isLastNode(root))
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i])
        {
            // append current character to currPrefix string
            currPrefix.push_back(97 + i);

            // recur over the rest
            suggestionsRec(root->children[i], currPrefix, elements);
            
            //should remove the last index, inserted in this loop.
            if (!currPrefix.empty()) {
                currPrefix.pop_back();
            }
        }
    }
}

// print suggestions for given query prefix.
int getSearchResults(TrieNode* root, const string query,  unordered_set<Element, elementHash>& elements)
{
    struct TrieNode* pCrawl = root;

    // Check if prefix is present and find the
    // the node (of last level) with last character
    // of given string.
    int level;
    int n = query.length();
    for (level = 0; level < n; level++)
    {
        int index = CHAR_TO_INDEX(query[level]);

        // no string in the Trie has this prefix
        if (!pCrawl->children[index])
            return 0;

        pCrawl = pCrawl->children[index];
    }

    // If prefix is present as a word.
    bool isWord = (pCrawl->isWordEnd == true);

    // If prefix is last node of tree (has no
    // children)
    bool isLast = isLastNode(pCrawl);

    // If prefix is present as a word, but
    // there is no subtree below the last
    // matching node.
    cout << "matches: " << endl;
    if (isWord && isLast)
    {
        cout << "Only one match" << endl;
        cout << query << endl;

        Element element = findName(elements, query);
        pair<string, string> temp_pair = make_pair(element.atomNum, element.name);
        
        predictions.push(temp_pair);  // adding pair to stack
        
        return -1;
    }

    // If there are are nodes below last
    // matching character.
    if (!isLast)
    {
        string prefix = query;
        suggestionsRec(pCrawl, prefix, elements);
        return 1;
    }
}

#endif /*TYPES_H*/
