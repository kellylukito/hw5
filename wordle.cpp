#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(string& current, const string& in, string remaining_float, const set<string>& dict, set<string>& result, size_t pos);



// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> result;
    string curr = in;
    wordleHelper(curr, in, floating, dict, result, 0);
    return result;


}

// Define any helper functions here
void wordleHelper(string& current, const string& in, string remaining_float, const set<string>& dict, set<string>& result, size_t pos){
    //base case. All positions full
    if(pos == in.length()){
        //make sure it is a word
        if(dict.find(current) != dict.end()){
            //make sure all floating character used
            if(remaining_float.empty()){
                result.insert(current);
            }
        }
        return;
    }

    //position already has filled in letter
    if(in[pos] != '-'){
        //if it is in remaining_float remove it
        if(remaining_float.find(in[pos])<remaining_float.length()){
            remaining_float.erase(remaining_float.find(in[pos]), 1);
        }
        wordleHelper(current, in, remaining_float, dict, result, pos+1);
    }
    //if position is not yet filled
    else{
        for(size_t i=0; i<remaining_float.length(); ++i){
            current[pos] = remaining_float[i];

            string new_remaining = remaining_float;
            new_remaining = new_remaining.erase(i,1);
            
            wordleHelper(current, in, new_remaining, dict, result, pos+1);
        }

        int remaining_slots = in.length()-pos-1;
        int fixed_slots = 0;

        for(size_t i=pos+1; i<in.length(); ++i){
            if(in[i] != '-'){
                fixed_slots++;
            }
        }
        int open_slots = remaining_slots - fixed_slots;

        //ensure remaining floating chars are used first
        if(open_slots >= int(remaining_float.length())){
            for(char c= 'a'; c <= 'z'; ++c){
                //skip if remaining char is there
                if(remaining_float.find(c) < remaining_float.length()){
                    continue;
                }
                current[pos] = c;
                wordleHelper(current, in, remaining_float, dict, result, pos + 1);
            }
        }
    }
}
