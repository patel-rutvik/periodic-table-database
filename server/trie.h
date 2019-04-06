/*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Final Project: Periodic Table Database
Names: Rutvik Patel, Kaden Dreger
ID: 1530012, 1528632
CCID: rutvik, kaden
CMPUT 275 Winter 2018

This file holds the trie data structure used to generate search predicitions.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
#ifndef TRIE_H
#define TRIE_H

/*We used a modified version of the trie structure implemented at:
https://www.geeksforgeeks.org/trie-insert-and-search/

*/
// Alphabet size
#define NUM_LETTERS 26

// Converts char to index
#define LETTER_TO_INDEX(c) ((int)c - (int)'a')

Element findName(unordered_set<Element, elementHash>& elements, string name);
vector<pair<string, string>> predictions;  // vector to store search predictions

/*This is the TrieNode struct*/
struct TrieNode {
    struct TrieNode *children[NUM_LETTERS];  // array of children
    bool endWord;  // end of word indicator
};

// Initializing NULLs
struct TrieNode *getNode(void) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The getNode function takes no parameters

It returns the parameters:
    pNode: the resulting TrieNode

The point of this function is to create a new node, initializing all its
children to NULL.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    struct TrieNode *pNode = new TrieNode;  // new node
    pNode->endWord = false;  // initialize end of word indicator to false
    for (int i = 0; i < NUM_LETTERS; i++) {
        pNode->children[i] = NULL;  // initialize children to NULL
    }
    return pNode;  // return the node
};

// Inserts key into tree, if key is a prefix mark leaf node
void insert(struct TrieNode *root, const string key) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The insert function takes the parameters:
    root: the current root of the Trie
    key: the string to be inserted

It returns no parameters.

The point of this function is to insert the given key into the Trie.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    struct TrieNode *pointSearch = root;  // initialize pointer to root

    /* for each character in the key... */
    for (unsigned int level = 0; level < key.length(); level++) {
        int index = LETTER_TO_INDEX(key[level]);  // convert letter to index
        if (!pointSearch->children[index]) {
            pointSearch->children[index] = getNode();  // initialize new node
        }
        // Point to the next child
        pointSearch = pointSearch->children[index];
    }

    // mark last node as leaf
    pointSearch->endWord = true;
}

bool search(struct TrieNode *root, const string key) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The search function takes the parameters:
    root: the current root of the Trie
    key: the string to be searched

It returns a bool true if found, false if not found.

The point of this function is to search the given key in the Trie.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    struct TrieNode *pointSearch = root;
    for (unsigned int level = 0; level < key.length(); level++) {
        int index = LETTER_TO_INDEX(key[level]);

        if (!pointSearch->children[index]) {
            // Key not present
            return false;
        }
        // Point to the next child
        pointSearch = pointSearch->children[index];
    }
    // Key is present return true
    return (pointSearch != NULL && pointSearch->endWord);
}

bool lastWordNode(struct TrieNode* root) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The lastWordNode function takes the parameters:
    root: the current root of the Trie

It returns a bool true if last word, false if subtree exists.

The point of this function is to check if the current root is the last word.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    for (int i = 0; i < NUM_LETTERS; i++) {
       if (root->children[i]) {
            // Current node has child
            return false;
        } 
    }
    // Children are NULL
    return true;
}

void getSuggestions(struct TrieNode* root, string currPrefix,
                    unordered_set<Element, elementHash>& elements)
{
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The getSuggestions function takes the parameters:
    root: the current root of the Trie
    currPrefix: the string representing the current prefix
    elements: the hash table holding all of the elements

It does not return any parameters.

The point of this function is to generate the predictions based on the current
prefix. Specifically, it generates and stores the top 4 results to a vector.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // found a string with the given prefix
    if (root->endWord) {
        cout << currPrefix << endl;

        Element element = findName(elements, currPrefix);  // find the element
        pair<string, string> temp_pair = make_pair(element.atomNum,
                                                   element.name);
        if (predictions.size() < 4) {  // if 4 elements have not been added
            predictions.push_back(temp_pair);  // adding pair to vector
        }
    }

    // All children struct node pointers are NULL
    if (lastWordNode(root)) {
        return;
    }

    for (int i = 0; i < NUM_LETTERS; i++) {
        if (root->children[i]) {
            // append character to string
            currPrefix.push_back(97 + i);

            // recurse over the remaining
            getSuggestions(root->children[i], currPrefix, elements);
            
            // should remove the last index, inserted in this loop.
            if (!currPrefix.empty()) {
                currPrefix.pop_back();
            }
        }
    }
}

void getSearchResults(TrieNode* root, const string query,
                      unordered_set<Element, elementHash>& elements) {
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
The getSearchResults function takes the parameters:
    root: the current root of the Trie
    query: the string to be searched
    elements: the hash table holding all of the elements

It returns no parameters.

The point of this function is to conduct the search process using the Trie
data structure implemented above.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    struct TrieNode* pointSearch = root;

    // Check if prefix is present and find the
    // the node (of last level) with last character
    // of given string.
    for (unsigned int level = 0; level < query.length(); level++) {
        int index = LETTER_TO_INDEX(query[level]);

        // no string has this prefix
        if (!pointSearch->children[index]) {
            return;
        }
        pointSearch = pointSearch->children[index];
    }

    // prefix is full word.
    bool fullWord = (pointSearch->endWord == true);

    // No more words can be formed
    bool lastWord = lastWordNode(pointSearch);

    cout << "Match(es): " << endl;
    if (fullWord && lastWord) {
        cout << query << endl;
        Element element = findName(elements, query);
        pair<string, string> temp_pair = make_pair(element.atomNum, element.name);
        predictions.push_back(temp_pair);  // adding pair to vector 
        return;
    }

    // More words can still be made
    if (!lastWord) {
        string prefix = query;
        getSuggestions(pointSearch, prefix, elements);  // Recurse over remaining
        return;
    }
}


#endif /*TRIE_H*/