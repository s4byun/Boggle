#include "boggleplayer.h"
#include <vector>
#include <string>
#include <set>
#include <locale>
#include <array>
#include <iostream>

using namespace std;

namespace global {
  int neighbors[] = {-1,-1,0,-1,1,0,1,1,-1}; // Used to traverse all neighboring dice
}

/**
 * Destructor for BogglePlayer.
 */
BogglePlayer::~BogglePlayer() {
  delete[] isVisited;
  delete[] array;

  isVisited = NULL;
  array = NULL;
}

/**
 * Builds a multiway trie with the words in the set word_list.
 * This trie will be used as our lexicon.
 */
void BogglePlayer::buildLexicon(const set<string>& word_list) {
  // Clear the already existing lexicon.
  lexicon.clear();

  // Add all the words in the word_list to the lexicon.
  for( auto it = word_list.begin(); it != word_list.end(); ++it ) {
    lexicon.insert(*it);
  }

  isBuildLexicon = true;
}

/**
 * Creates the boggle board with given dimensions (rows x cols) and dice.
 */
void BogglePlayer::setBoard(unsigned int rows, unsigned int cols, string** diceArray) {
  BOARDSIZE = rows*cols;  // Total number of dice
  ROWS = rows;
  COLS = cols;

  /* If any of these arrays have been initialized before, reset them. */
  if(array)
    delete[] array;
  if(isVisited)
    delete[] isVisited;

  array = new string[BOARDSIZE];
  isVisited = new bool[BOARDSIZE];
  
  // Creates a 1D array representation of the board.
  for(int index=0; index < BOARDSIZE; index++) {
    array[index] = diceArray[index / COLS][index % COLS];

    // Converts the string to all lower case.
    for(unsigned int i = 0; i < array[index].size(); i++) 
      array[index][i] = tolower(array[index][i]);
  }

  isSetBoard = true;
}

/**
 * Finds all words that are both in the lexicon and on the board, and that
 * have at least minimum_word_length number of characters.
 *
 * Returns false if the board hasn't been set yet or if the lexicon hasn't
 * been built yet.
 *
 * All the valid words are stored in the set "words"
 */
bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words) {
  if(!isSetBoard) {
    return false;
  }
  if(!isBuildLexicon){
    return false;
  }

  string word;

  // Initialize all visited values to false.
  for(int i=0; i<BOARDSIZE; i++) {
    isVisited[i] = false;
  }

  // Traverse the board using every die as the starting point of the traversal.
  for(int i = 0; i < BOARDSIZE; i++) {
    word = array[i];

    // If the die's face is already a valid word, insert it in the set.
    if(word.size() >= minimum_word_length && isInLexicon(word)) {
      words->insert(word);
    }

    // If the die's face is a prefix of any word in the lexicon.
    if(lexicon.find(word) != -1) {
      isVisited[i] = true;

      // Get all valid words on the board, whose prefix is the current die.
      traverseBoard(minimum_word_length, i, word, words);
      isVisited[i] = false; 
    }
  }

  return true;
}

/**
 * Checks if the given word is in the lexicon.
 * Returns true if it is.
 */
bool BogglePlayer::isInLexicon(const string& word_to_check) {
  if(lexicon.find(word_to_check) == 1) 
    return true;

  return false;
}

/**
 * Checks if the given word can be found on the board; 
 * if yes, returns the search path; otherwise, returns empty vector. 
 */
vector<int> BogglePlayer::isOnBoard(const string& word_to_check) {
  vector<int> path;

  // If the board isn't set yet, return empty vector.
  if(!isSetBoard) {
    return path;
  }

  // Initialize all visited values to false.
  for(int i=0; i < BOARDSIZE; i++) {
    isVisited[i] = false;
  }

  string lc_word_to_check = word_to_check;  // The word in all lower case

  // Converts the word to all lower case.
  for(unsigned int i=0; i < word_to_check.size(); i++)
    lc_word_to_check[i] = tolower(word_to_check[i]);

  // Goes through each die as a starting search point.
  for(int index = 0; index < BOARDSIZE; index++) {
    // If the die's face is a prefix of the word.
    if(isPrefix(array[index], lc_word_to_check)) {
      path.push_back(index);

      // If the word is already found at this point.
      if(array[index].size() == word_to_check.size())
        break;

      // If the word is on the board.
      if(isOnBoardRecurse(path, lc_word_to_check.substr(array[index].size()))) 
        break;

      /*
       * If the word is not found, go to the
       * next die as a new starting point.
       */
      path.pop_back();
    }
  }

  return path;
}

/**
 * Recursively searches through the board and returns true if the word is found.
 */
bool BogglePlayer::isOnBoardRecurse(vector<int>& path, const string& word) {
  int row = path.back() / COLS;   // This die's row
  int col = path.back() % COLS;   // This die's col

  // Loops through every neighboring die.
  for(int i=0; i < N_SIZE; i++) {
    int nrow = row + global::neighbors[i];   // Neighbor die's row
    int ncol = col + global::neighbors[i+1]; // Neighbor die's col

    // If that cell is on the board.
    if(isValid(nrow,ncol)) {
      int nindex = nrow*COLS + ncol;  // Index of the neighbor die

      // If the die has not been visited yet.
      if(!isVisited[nindex]) {
        // If the die's face is a prefix of the word.
        if(isPrefix(array[nindex], word)) {
          path.push_back(nindex);
          isVisited[nindex] = true;

          // If the word is found on the board at this point.
          if(word.size() == array[nindex].size()) return true;

          // If the word is found on the board.
          if(isOnBoardRecurse(path, word.substr(array[nindex].size()))) return true;

          // If the word is not found, go to the rest of the neighbors.
          path.pop_back();
          isVisited[nindex] = false;
        }
      }
    }
  }

  return false; // Word is not found. 
}

/**
 * Recursive board traversal used for getAllValidWords.
 */
void BogglePlayer::traverseBoard(const unsigned int& min, int index, string word, set<string>* words) {
  int row = index / COLS;   // Current die's row
  int col = index % COLS;   // Current die's col

  // Loop through every neighboring die.
  for(int i=0; i < N_SIZE; i++) {
    int nrow = row + global::neighbors[i];    // Neighbor die's row
    int ncol = col + global::neighbors[i+1];  // Neighbor die's col

    // If this neighbor is in the board.
    if(isValid(nrow,ncol)) {
      int nindex = nrow*COLS + ncol;  // This neighbor's index

      // If this neighbor hasn't been visited yet.
      if(!isVisited[nindex]) {
        // The word formed by following the search path up to this point.
        string nword = word;
        nword.append(array[nindex]);  

        int result = lexicon.find(nword);

        // If the word is not in the lexicon, even as a prefix of another word.
        if(result == -1) {
          continue;
        }

        // If the word is a full word found in the lexicon, insert in the set.
        if(result == 1 && nword.size() >= min) {
          words->insert(nword);
        }

        isVisited[nindex] = true;

        // Go to its neighbors.
        traverseBoard(min, nindex, nword, words);
        isVisited[nindex] = false;
      }
    }
  }
}

// Returns true if "prefix" is a prefix of the "word".
bool BogglePlayer::isPrefix(const string& prefix, const string& word) {
  return word.substr(0,prefix.size()).compare(prefix) == 0; 
}

// Returns true if a given die is on the board.
bool BogglePlayer::isValid(int row, int col) {
  return (row >= 0) && (col >= 0) && (row < ROWS) && (col < COLS);
}

/**
* Makes a 4x4 board, all dice of which have "a" on their faces.
*/
void BogglePlayer::getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols) {
 new_board = new string*[4];

 for(int i=0; i<4; i++) {
   new_board[i] = new string[4];
 }

 for(int i=0; i<4; i++) {
   for(int j=0; j<4; j++) {
     new_board[i][j] = "a";
   }
 }

 *rows = 4;
 *cols = 4;
}
