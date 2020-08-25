/*
 * ActorGraph.cpp
 * Author: Jiahe Liu
 * Date:   06/01/2019
 * Email: jil061@ieng6.ucsd.edu
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <climits>
#include <queue>
#include "ActorGraph.hpp"
#define year 2
#define arg 3
using namespace std;;


/**
 * Constructor of the Actor graph
 */ 


/** You can modify this method definition as you wish
 *
 * Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename - input filename
 * use_weighted_edges - if true, compute edge weights as 1 + (2019 - movie_year),
 *                      otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */

/* Read file and build the graph */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        
        // get the next line
        if (!getline( infile, s )) break;
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != arg) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[year]);
        // TODO: we have an actor/movie relationship, now what?
        string nameYear = movie_title + "#@" + record[year];
        // Used for weighted actor Node added
        if (actor.find(actor_name) == actor.end() && use_weighted_edges){
            Node* newActor = new Node;
            newActor->actorName = actor_name;
            newActor->dist = INT_MAX;
            List* temp = new List();
            temp->nameAndYear = nameYear;
            temp->weight = 1 + (2019 - movie_year);
            newActor->movie.push_back(temp);
            actor.insert({actor_name, newActor});
        }
        // Used for unweighted actor Node added
        else if (actor.find(actor_name) == actor.end() && !use_weighted_edges){
            Node* newActor = new Node; 
            newActor->actorName = actor_name;
            newActor->dist = INT_MAX;
            List* temp = new List();
            temp->nameAndYear = nameYear;
            temp->weight = 1;
            newActor->movie.push_back(temp);
            actor.insert({actor_name, newActor});
        }

        unordered_map<string, Node*>::iterator itr = actor.find(actor_name); 
        int movieListSize = itr->second->movie.size();
        int moviePosition = -1;
        // Find the movie position 
        for(int i = 0; i < movieListSize; i++){
            if(itr->second->movie.at(i)->nameAndYear == nameYear){
                moviePosition = i;
                break;
            } 
        }
        // If no movie found, creat a new Node
        if (moviePosition == -1){
            List* temp = new List();
            temp->nameAndYear = nameYear;
            temp->weight = 1 + (2019 - movie_year);
            itr->second->movie.push_back(temp);
            moviePosition = movieListSize;
        }
        if (movieMap.find(nameYear) == movieMap.end()){
            vector<string> newMovie;
            newMovie.push_back(actor_name);
            movieMap.insert({nameYear, newMovie});
        }
        else{
            movieMap.find(nameYear)->second.push_back(actor_name);
        }
        // Build the graph connected together
        vector<string> actorList = movieMap.find(nameYear)->second;
        int size = actorList.size();
        for(int i = 0; i < size; i++){
            if(actorList[i] != actor_name){
                unordered_map<string, Node*>::iterator compareItr =
                actor.find(actorList[i]);
            
                int compareSize = compareItr->second->movie.size();
                int comparePosition;
                for(int j = 0; j < compareSize; j++){
                    if(compareItr->second->movie.at(j)->nameAndYear ==
                     nameYear){
                        comparePosition = j;
                        break;
                    }  
                }
                itr->second->movie.at(moviePosition)->
                connection.push_back(compareItr->second);
                compareItr->second->movie.at(comparePosition)->
                connection.push_back(itr->second); 
            }
        }
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    cout << "done" << endl;

    return true;
}

/* Find the unweighted shortest path and write out into a file*/
void ActorGraph::find(const char* in_filename, const char* out) {

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
    
    ofstream output(out);
    output << "(actor)--[movie#@year]-->(actor)--..." << endl;
 
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {

            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
        if (record.size() != year ) {
            // we should have exactly 2 columns
            continue;
        }

        string actor1(record[0]);
        string actor2(record[1]);
        cout << "Computing path for (" << actor1 << ") -> (" <<
            actor2 << ")" << endl;
        if (actor.find(actor1) == actor.end()){
            cout << "Failed to locate node '" + actor1 + "'" << endl;
        }
        else if (actor.find(actor2) == actor.end()){
            cout << "Failed to locate node '" + actor2 + "'" << endl;
        }
        else{
            Node* act1 = actor.find(actor1)->second;
            uFind(act1, actor2);    // Store the path into a priority queue;
            outputW(output, actor2, actor1);    // Output the path
        }
        resetVisit();   // Reset all node
    }
        infile.close();
        output.close();
}


/* Find the shortest path by BFS and store into a priority queue */
void ActorGraph::uFind(Node* theActor, string targetName){
    theActor->dist = 0; // Set the first node dist to 0
    bool check = false; // A marker if shortest found then break
    pq.push(make_pair(0, theActor));
    while(!pq.empty()){ 
        Node* temp = pq.top().second;
        pq.pop();
        if(!temp->visit){
            temp->visit = true;
            int mSize = temp->movie.size();
            for (int i = 0; i < mSize; i++){
                int cSize = temp->movie[i]->connection.size();
                for (int j = 0; j < cSize; j++){
                    Node * neighbor = temp->movie[i]->connection[j];
                    if (neighbor->dist == INT_MAX){
                        neighbor->dist = temp->dist + 1;
                        neighbor->prev = temp;
                        neighbor->movieN = temp->movie[i]->nameAndYear;
                        pq.push({temp->dist, neighbor});
                        if (neighbor->actorName == targetName){ // Found,break
                            int n = pq.size();
                            for (int i = 0; i < n; i++)
                            pq.pop();
                            check = true;
                            break;
                        }
                    }
                    if (check) break;
                }
                if (check) break;
            }
        }
        if (check) break;
    } 
}

/* Found the weighted shortest way by Dij method */
void ActorGraph::wFind(const char* in_filename, const char* out){

    ofstream output(out);
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
    
   // ofstream output(out);
 
    output << "(actor)--[movie#@year]-->(actor)--..." << endl;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {

            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
        if (record.size() != year ) {
            // we should have exactly 2 columns
            continue;
        }

        string actor1(record[0]);
        string actor2(record[1]);
        
        cout << "Computing path for (" << actor1 << ") -> (" <<
            actor2 << ")" << endl;

        if (actor.find(actor1) == actor.end()){
            cout << "Failed to locate node '" + actor1 + "'" << endl;
        }
        else if (actor.find(actor2) == actor.end()){
            cout << "Failed to locate node '" + actor2 + "'" << endl;
        }
        else{
            Node* act1 = actor.find(actor1)->second;
            dij(act1, actor2);  //Dij method store the path to a pritory queue
            outputW(output, actor2, actor1); // Output the path
        }
        resetVisit();   // Reset all Node
    
    }
    infile.close(); 
    output.close();
}

/*  Dij Method to find the shortest weighted path */
void ActorGraph::dij(Node* theActor, string targetName){

    pq.push(make_pair(0, theActor));
    theActor->dist = 0;
    while (!pq.empty()){
        Node* temp = pq.top().second;
        if (pq.top().second->actorName == targetName){  // Shortest found
            int n = pq.size();
            for (int i = 0; i < n; i++)
                pq.pop();
            break;
        }
        pq.pop();
        if(!temp->visit){
            temp->visit = true;
            int mSize = temp->movie.size();
            for (int i = 0; i < mSize; i++){
                int cSize = temp->movie[i]->connection.size();
                for (int j = 0; j < cSize; j++){
                    int c = temp->dist + temp->movie[i]->weight;
                    if (c < temp->movie[i]->connection[j]->dist){
                        temp->movie[i]->connection[j]->prev = temp;
                        temp->movie[i]->connection[j]->dist = c;    
                        temp->movie[i]->connection[j]->movieN =
                        temp->movie[i]->nameAndYear;
                        pq.push(make_pair(c, temp->movie[i]->connection[j]));
                    }
                }
            }
        }
    }
}

/*  Output the path into a file */
void ActorGraph::outputW(ofstream &output, string name, string start){

    Node* s = actor.find(start)->second;
    Node* target = actor.find(name)->second;
    vector<string> answer;
    answer.push_back(name);
    while (target != s){
        string movieName = target->movieN;
        answer.push_back(movieName);
        string nextName = target->prev->actorName;
        answer.push_back(nextName);
        target = target->prev;
    } 
    
    int sizeV = answer.size();
    if (sizeV == 1) {
        output << "";
        answer.pop_back();
    }
    else{
        for (int i = 0; i < sizeV; i++){
            if(i % year == 0){ 
                output << "(" << answer.back() << ")";
                answer.pop_back();   
            }
            else{
                output << "--[" << answer.back() << "]-->";
                answer.pop_back();
            }
        }
    }
    output << endl;
}

/* Reset all node */
void ActorGraph::resetVisit(){
    for (auto itr = actor.begin(); itr != actor.end(); itr++){
        itr->second->visit = false;
        itr->second->prev = NULL;
        itr->second->dist = INT_MAX;
    }
}

/* MovieTraveler build the edge */
bool ActorGraph::buildEdge(const char* in_filename){

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        
        // get the next line
        if (!getline( infile, s )) break;
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != arg) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[year]);
        string nameYear = movie_title + "#@" + record[year];

        if (actorList.find(actor_name) == actorList.end()){
            Node* newNode = new Node;
            newNode->actorName = actor_name;
            newNode->parent = newNode;
            newNode->height = 1;
            actorList.insert({actor_name, newNode});
        }
        if (movieList.find(nameYear) == movieList.end()){
            vector<string> newMovie;
            newMovie.push_back(actor_name);
            movieList.insert({nameYear, newMovie});
        }
        else{
            movieList.find(nameYear)->second.push_back(actor_name);
        }
    }
    infile.close();
    // store all edges
    for (auto itr = movieList.begin(); itr != movieList.end(); itr++){
        
        string movieName = itr->first;
        int at;
        int nAt = count(movieName.begin(), movieName.end(), '@');
        int weight;
        if (nAt == 1){
            at = movieName.find('@');
            string stringYear = movieName.substr(at+1, 4);
            int mYear = atoi(stringYear.c_str());      
            weight = 1 + (2019 - mYear);
        }
        else{
            string temp = movieName;
            for(int i = 1; i < nAt; i++){
                int p = temp.find('@');
                temp.erase(0, p+1);
            }
            at = temp.find('@');
            string stringYear = temp.substr(at+1, 4);
            int mYear = atoi(stringYear.c_str());      
            weight = 1 + (2019 - mYear);
        }

        int size = itr->second.size();
        for (int i = 0; i < size; i++){
            for(int j = i + 1; j < size; j++){
                edge e;
                e.first = weight;
                e.second ={movieName, {actorList.find(itr->second[i])->second, 
                    actorList.find(itr->second[j])->second}};
                p.push(e);  // edge stored
            }
        }
    }
    totalV = actorList.size();  // Total vertex
}

/* find the toppest node name */
Node* ActorGraph::find(Node* a){
    if (a->parent != a)
        a = find(a->parent);
    return a;
}


/* MST method output all the edges */
void ActorGraph::mst(string output){
    ofstream out(output);
    out << "(actor)<--[movie#@year]-->(actor)" << endl;
    
    int count = 0;
    while(!p.empty()){
        Node* one = p.top().second.second.first;
        Node* two = p.top().second.second.second;

        Node* oneP = find(one);
        one->parent = oneP; // Compress
        Node* twoP = find(two);
        two->parent = twoP; // Compress
        if(oneP != twoP){
            out << "(" << one->actorName << ")";
            out << "<--[" << p.top().second.first << "]-->";
            out << "(" << two->actorName << ")" << endl;

            count = count + p.top().first;
            // Merge
            if (oneP->height > twoP->height)
                twoP->parent = oneP; 
            else
                oneP->parent = twoP;
            if (oneP->height == twoP->height)
                twoP->height++;
        }
        p.pop();
    }
    out << "#NODE CONNECTED: " << totalV << endl;
    out << "#EDGE CHOSEN: " << totalV - 1 << endl;
    out << "TOTAL EDGE WEIGHTS: " << count << endl;
    out.close();
}


/* Destuctor */
ActorGraph::~ActorGraph(){
    for(auto itr = actor.begin(); itr != actor.end(); itr++){
        int size = itr->second->movie.size();
        for(int i = 0; i < size; size--){
            int cSize = itr->second->movie.at(size-1)->connection.size();
            for (int j = 0; j < cSize; j++){
               itr->second->movie.at(size-1)->connection.pop_back(); 
            }
            delete itr->second->movie.back();
            itr->second->movie.pop_back();
        }
        delete itr->second;
    }
    // Delete all pointers
    for (auto itr = actorList.begin(); itr != actorList.end(); itr++){
       delete itr->second; 
    }
}

