#include "boggleutil.h"

#include <iostream>

/**
 * Recursively clears all nodes in the trie.
 */
void Node::clearAll(Node* node) {
  // Loop through the node's map and recurse from there.
  for(auto it=node->map.begin(); it != node->map.end(); it++) {
    node->clearAll(it->second);
  }
  node->map.clear();

  delete node;
  node = NULL;
}

/**
 * Clears the trie.
 */
void WordTrie::clear() {

  // Delete all subtries of root 
  for(auto it = root->map.begin(); it != root->map.end(); it++) {
    root->clearAll(it->second);
  }

  root->map.clear();
}

/**
 * Searches for the given word in the trie.
 * 
 * Return 1 if the given word is a whole word in the trie.
 * Return 0 if the given word is a prefix of another word.
 * Return -1 if the given word is not in the trie at all.
 */
int WordTrie::find(const std::string& pWord) {
  Node *cNode = root;

  // Loop to traverse the trie character by character.
  for(unsigned int i=0; i < pWord.size(); i++) {
    auto find = cNode->map.find(pWord[i]);

    // If the word is not in the trie.
    if(find == cNode->map.end()) 
      return -1;
    
    // If the current char is found, find the next character.
    else {
      cNode = find->second;
    }
  }

  return cNode->end;
}

/**
 * Inserts a word into the trie.
 */
void WordTrie::insert(std::string pWord) {
  Node* cNode = root;
  char c;

  // Loop to go through each character in the word.
  for(unsigned int i=0; i < pWord.size(); i++) {
    c = tolower(pWord[i]);
    auto find = cNode->map.find(c);

    // If the current character is not found along the path.
    if(find == cNode->map.end()) {
      std::pair<char, Node*> newNode(c, new Node());

      // Insert a new node pointing from the current char.
      cNode->map.insert(newNode);

      // Go down the trie.
      cNode = cNode->map[c];
    } 

    // If the current char is found along the path, just keep going down.
    else {
      cNode = find->second;
    }
  }

  cNode->end = 1; // Mark the end of the word.
}

/**
 * Destructor for the trie.
 */
WordTrie::~WordTrie() {
  clear();
  delete root;
  root = NULL;
}
