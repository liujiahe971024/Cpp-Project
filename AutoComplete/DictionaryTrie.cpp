#include "DictionaryTrie.hpp"
#include <unordered_map>
#include <iostream>
#include <utility>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;
/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie()
{
    this->root = new unordered_map<char, TriNode*>();
    this->threshold = 0; // Suppose to find second largest freq
    end = true;
}

/** 
 * Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string). This might be useful for testing
 * when you want to test a certain case, but don't want to
 * write out a specific word 300 times.
 */
bool DictionaryTrie::insert(std::string word, unsigned int freq)
{
    if( word.empty() )
        return false;
    unordered_map<char, TriNode*>* temp = this->root;
    for( unsigned int i = 0; i < word.length(); i++ ){
        if( temp->find(word.at(i)) != temp->end() ){
            if( i == word.length()-1 ){
                if( temp->find(word.at(i))->second->frequency == 0 )
                    temp->find(word.at(i))->second->frequency = freq;
                else
                    return false;
            } 
            if(freq > temp->find(word.at(i))->second->maxFreq)
               temp->find(word.at(i))->second->maxFreq = freq;  // Set MaxFreq 
            temp = temp->find(word.at(i))->second->next;
        }
        else{
            if (i != word.length()-1){
                temp->insert(pair<char, TriNode*>(word.at(i), new TriNode(0))); 
            // Changed freq to 0 --LK
                if(freq > temp->find(word.at(i))->second->maxFreq)
                    temp->find(word.at(i))->second->maxFreq = freq;  // Set MaxFreq
                temp = temp->find(word.at(i))->second->next;
                 
            }
            else{
                temp->insert(pair<char, TriNode*>(word.at(i), 
                    new TriNode(freq)));

                if(freq > temp->find(word.at(i))->second->maxFreq)
                    temp->find(word.at(i))->second->maxFreq = freq;
                temp = temp->find(word.at(i))->second->next;
            }
        }
    }
    return true;
}


/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryTrie::find(std::string word) const
{
    if( word.empty() )
        return false;
    unordered_map<char, TriNode*>* temp = this->root;
    for( unsigned int i = 0; i < word.length(); i++ ){
        if( temp->find(word.at(i)) != temp->end() ){
            if( i == word.length()-1 && 
                temp->find(word.at(i))->second->frequency != 0){ 
                                         //Added frequency compare --LK
                return true;
            }
            temp = temp->find(word.at(i))->second->next;
        }
    }
    return false;
}


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
std::vector<std::string> DictionaryTrie::predictCompletions(std::string prefix,
     unsigned int num_completions)
{
    std::vector<std::string> myVector;
    myVector.clear();
    tempList.clear();
    end = true;
    myQueue = 
    priority_queue<pair<string,int>, vector<pair<string,int>>, compareFreq>(); 
    if( prefix.empty() || num_completions == 0 )
        return myVector;
    unordered_map<char, TriNode*>* temp = this->root;
    unordered_map<char, TriNode*>::iterator itr;
    for( unsigned int i = 0; i < prefix.length(); i++ ){
        itr = temp->find(prefix.at(i));
        if( itr == temp->end() )
           return myVector;
        if( i == prefix.length() - 1 && itr->second->frequency != 0 )
            myQueue.push(make_pair(prefix, itr->second->frequency));
        temp = itr->second->next;
    }
    string str = prefix;

    while(myQueue.size() < num_completions && end == true){
        helper(temp, str);
    }
    
    helper(temp, str);

    if (myQueue.size() < num_completions)
            num_completions = myQueue.size();
    for( unsigned int i = 0; i < num_completions; i++ ){

        myVector.push_back(myQueue.top().first);
        string tempWord = myQueue.top().first;
        unsigned int tempFre = myQueue.top().second;
        reinsert(tempWord, tempFre);
        myQueue.pop();
    }
    return myVector;
} 
// Suppose to find the maxFreq and equally by one time recursion
void DictionaryTrie::helper(unordered_map<char, TriNode*>* temp, string str){ 
        unordered_map<char, TriNode*>* curr = temp;
        unordered_map<char, TriNode*>::iterator itr = curr->begin();
        int maxIndex = -1;
        unsigned int maxFrequency=0;
        for(int i = 0; itr != curr->end(); i++, itr++){
            if(maxFrequency < itr->second->maxFreq){
              maxFrequency = itr->second->maxFreq;
              maxIndex = i;
             }
        }
        itr = curr->begin();
        if (maxIndex != -1){
            for (int i = 0; i < maxIndex; i++)
               itr++;
            str = str + itr->first;
            if(!(itr->second->next->empty())){
                if(itr->second->maxFreq == itr->second->frequency 
                    && !stored(str)){
                    tempList.push_back(str);
                    myQueue.push(make_pair(str, itr->second->frequency));
                    unordered_map<char, TriNode*>* 
                            nextLvCurr = itr->second->next;
                    unordered_map<char, TriNode*>::iterator 
                            nextLvitr = nextLvCurr->begin();
                    unsigned int nextLvMaxFreq = 0;
                    for (int i = 0; 
                        nextLvitr != nextLvCurr->end(); i++, nextLvitr++){
                        if(nextLvMaxFreq < nextLvitr->second->maxFreq)
                            nextLvMaxFreq = nextLvitr->second->maxFreq;
                    }
                    itr->second->maxFreq = nextLvMaxFreq;
                }
                else{
                    helper(itr->second->next, str);
                
                    if(threshold < itr->second->frequency && !stored(str)){
                        itr->second->maxFreq = itr->second->frequency;
                    }
                    else
                        itr->second->maxFreq = threshold;
                }
            }
            else{
            tempList.push_back(str);
            myQueue.push(make_pair(str, itr->second->frequency));
            itr->second->maxFreq = 0;
            }
            threshold = 0;
            itr = curr->begin();
            for(int i = 0; itr != curr->end(); i++, itr++){
                if(threshold < itr->second->maxFreq){
                    threshold = itr->second->maxFreq;
                }
            }
        }
        else
            end = false;
}
bool DictionaryTrie::stored(string temWord){
    for (unsigned int i = 0; i < tempList.size(); i++){
        if (tempList[i] == temWord) return true;
    }
    return false;
}


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

void DictionaryTrie::reinsert(string word, unsigned int freq){
    unordered_map<char, TriNode*>* temp = this->root;
    for (unsigned int i = 0; i < word.length(); i++){
            
             if (freq > temp->find(word.at(i))->second->maxFreq){
                temp->find(word.at(i))->second->maxFreq = freq;
             }
             temp = temp->find(word.at(i))->second->next;
    }
}

std::vector<string> DictionaryTrie::predictUnderscore(std::string pattern,
 unsigned int num_completions)
{
    std::vector<string> myVector;
    myVector.clear();
    myQueue = 
    priority_queue<pair<string,int>, vector<pair<string,int>>, compareFreq>(); 
    unordered_map<char, TriNode*>* temp = this->root;
    unordered_map<char, TriNode*>::iterator itr = temp->begin();
    string str;
    string prefixStr;
    unsigned int tempFreq;
    unsigned int prefixPosition = pattern.find("_");
    for (unsigned int i = 0; i < prefixPosition; i++){
        str = str + pattern.at(i);
        temp = temp->find(pattern.at(i))->second->next;
        itr = temp->begin();
        prefixStr = str;
    }
    if (prefixPosition == pattern.length()-1){
        for (; itr != temp->end(); itr++){
            if (itr->second->frequency != 0){
                str = str + itr->first;
                myQueue.push(make_pair(str, itr->second->frequency));
                str.pop_back();
            }
                
        }
    }
    else{ 
        unordered_map<char, TriNode*>* curr = temp;
        itr = curr->begin();
        for(; itr != temp->end(); itr++){
            curr = temp;
            str = str + itr->first;
            curr = itr->second->next;
            for (unsigned int i = prefixPosition + 1; i < pattern.length(); i++){
                if (curr->find(pattern.at(i)) != curr->end()){
                    str = str + pattern.at(i);
                    tempFreq = 
                    curr->find(pattern.at(i))->second->frequency;
                    if (str.length() == pattern.length() &&
                        tempFreq != 0){
                        myQueue.push(make_pair(str, tempFreq));
                    }
                    if (curr->find(pattern.at(i))->second->next){
                        curr = curr->find(pattern.at(i))->second->next;
                    }
                    else break;
                }
                else
                    break;
            }
            str = prefixStr;
        } 
    }
    
    if (myQueue.size() < num_completions)
            num_completions = myQueue.size();
    for( unsigned int i = 0; i < num_completions; i++ ){
        myVector.push_back(myQueue.top().first);
        myQueue.pop();
    }
    return myVector;
}

/* Destructor */
DictionaryTrie::~DictionaryTrie()
{
   for( auto itr = this->root->begin(); itr != this->root->end(); itr++)
        delete itr->second;
    delete this->root; 
}
