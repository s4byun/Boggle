#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H

#include <string>
#include <unordered_map>
#include <regex>
#include <set>

/**
 * Node of the multiway trie.
 */
class Node {
  public:
    void clearAll(Node* node);
    int end = 0;  // 0 = prefix, 1 = end of a word
    
    // Map that stores subtrees.
    std::unordered_map<char, Node*> map;
};

/**
 * Multiway Trie to store all the words in the lexicon.
 */
class WordTrie {
  public:
    Node* root;
    WordTrie() {
      root = new Node();
    }
    ~WordTrie();
    void clear();
    void insert(std::string);
    int find(const std::string&);
};

#endif /* BOGGLEUTIL_H */
