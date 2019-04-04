#ifndef TRIE_H
#define TRIE_H


// Alphabet size
#define NUM_LETTERS 26

// Converts char to index
#define LETTER_TO_INDEX(c) ((int)c - (int)'a')

Element findName(unordered_set<Element, elementHash>& elements, string name);
vector<pair<string, string>> predictions;  // vector to store search predictions

struct TrieNode {
    struct TrieNode *children[NUM_LETTERS];
    bool endWord;
};

// Initializing NULLs
struct TrieNode *getNode(void) {
    struct TrieNode *pNode = new TrieNode;
    pNode->endWord = false;
    for (int i = 0; i < NUM_LETTERS; i++) {
        pNode->children[i] = NULL;
    }
    return pNode;
};

// Inserts key into tree, if key is a prefix mark leaf node
void insert(struct TrieNode *root, const string key) {
    struct TrieNode *pointSearch = root;

    for (unsigned int level = 0; level < key.length(); level++) {
        int index = LETTER_TO_INDEX(key[level]);
        if (!pointSearch->children[index]) {
            pointSearch->children[index] = getNode();
        }
        // Point to the next child
        pointSearch = pointSearch->children[index];
    }

    // mark last node as leaf
    pointSearch->endWord = true;
}

bool search(struct TrieNode *root, const string key) {
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
    for (int i = 0; i < NUM_LETTERS; i++) {
       if (root->children[i]) {
            // Current node has child
            return false;
        } 
    }
    // Children are NULL
    return true;
}

void getSuggestions(struct TrieNode* root, string currPrefix,  unordered_set<Element, elementHash>& elements)
{
    // found a string with the given prefix
    if (root->endWord) {
        cout << currPrefix << endl;

        Element element = findName(elements, currPrefix);
        pair<string, string> temp_pair = make_pair(element.atomNum, element.name);
        if (predictions.size() < 4) {
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

            // recurse over remaining
            getSuggestions(root->children[i], currPrefix, elements);
            
            // should remove the last index, inserted in this loop.
            if (!currPrefix.empty()) {
                currPrefix.pop_back();
            }
        }
    }
}

void getSearchResults(TrieNode* root, const string query,  unordered_set<Element, elementHash>& elements) {
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