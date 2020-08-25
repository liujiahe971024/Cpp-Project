/**
 *  CSE 100 PA2 C++ Autocomplete
 *  Authors: Jor-el Briones, Christine Alvarado
 */

#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP
#include "TriNode.hpp"
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;
/**
 *  The class for a dictionary ADT, implemented as a trie
 *  You may implement this class as either a mulit-way trie
 *  or a ternary search trie, but you must use one or the other.
 */
class compareFreq{
public:
    bool operator()( pair<string, int> p1, pair<string, int> p2 ){
        if( p1.second < p2.second )
            return true;
        if( p1.second == p2.second && p1.first > p2.first )
            return true;
        return false;
    }
};  
class DictionaryTrie
{
public:

  /* Create a new Dictionary that uses a Trie back end */
  DictionaryTrie();

  /** 
   * Insert a word with its frequency into the dictionary.
   * Return true if the word was inserted, and false if it
   * was not (i.e. it was already in the dictionary or it was
   * invalid (empty string). This might be useful for testing
   * when you want to test a certain case, but don't want to
   * write out a specific word 300 times.
   */
  bool insert(std::string word, unsigned int freq);

  /* Return true if word is in the dictionary, and false otherwise. */
  bool find(std::string word) const;

  /* 
   * Return up to num_completions of the most frequent completions
   * of the prefix, such that the completions are words in the dictionary.
   * These completions should be listed from most frequent to least.
   * If there are fewer than num_completions legal completions, this
   * function returns a vector with as many completions as possible.
   * If no completions exist, then the function returns a vector of size 0.
   * The prefix itself might be included in the returned words if the prefix
   * is a word (and is among the num_completions most frequent completions
   * of the prefix)
   */
  std::vector<std::string>
  predictCompletions(std::string prefix, unsigned int num_completions);

  /* Return up to num_completions of the most frequent completions
   * of the pattern, such that the completions are words in the dictionary.
   * These completions should be listed from most frequent to least.
   * If there are fewer than num_completions legal completions, this
   * function returns a vector with as many completions as possible.
   * If no completions exist, then the function returns a vector of size 0.
   * The pattern itself might be included in the returned words if the pattern
   * is a word (and is among the num_completions most frequent completions
   * of the pattern)
   */
  std::vector<std::string>
  predictUnderscore(std::string pattern, unsigned int num_completions);

  /* Destructor */
  ~DictionaryTrie();

private:
  // Add your own data members and methods here
  unordered_map<char, TriNode*>* root;
  vector<string> tempList;
  void reinsert(string word, unsigned int freq);
  bool stored(string);
  priority_queue<pair<string,int>, vector<pair<string, int>>, compareFreq> myQueue;
  unsigned int threshold;
  bool end;
  void helper(unordered_map<char, TriNode*>* temp, string str);
};

#endif // DICTIONARY_TRIE_H
