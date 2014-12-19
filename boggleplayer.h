#ifndef BOGGLEPLAYER_H
#define BOGGLEPLAYER_H

#define N_SIZE 8 
#include "baseboggleplayer.h"
#include "boggleutil.h"
#include <set>
#include <vector>
#include <string>

class BogglePlayer : public BaseBogglePlayer {
  public:
    void buildLexicon(const set<string>& word_list);
    void setBoard(unsigned int rows, unsigned int cols, string** diceArray);
    bool getAllValidWords(unsigned int minimum_word_length, set<string>* words);
    bool isInLexicon(const string& word_to_check);
    vector <int> isOnBoard(const string& word_to_check);
    void getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols);
    ~BogglePlayer();

  private:
    bool isSetBoard, isBuildLexicon;  
    int BOARDSIZE, COLS, ROWS;  // Total number of dice, columns, and rows
    bool* isVisited;  // Keeps track of visited boolean of each die
    string* array;    // Our 1D representation of the board
    WordTrie lexicon; /* Multiway trie representing the lexicon. */
    
    /**
     * Returns true if "prefix" is a prefix of "word".
     */
    bool isPrefix(const string& prefix, const string& word);

    /**
     * Returns true if the die at (row, col) is actually on the board.
     */
    bool isValid(int row, int col); 

    /**
     * Recursive board traversal used by isOnBoard.
     */
    bool isOnBoardRecurse(vector<int>& path, const string& word);

    /**
     * Recursive board traversal used by getAllValidWords.
     */
    void traverseBoard(const unsigned int& min, int index, string word, set<string> *words);
};
#endif // BOGGLEPLAYER_H
