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

// counts the number of empty slots in a given word pattern string
// returns an integer representing the count
int countEmpty(const std::string& in);

// recursive helper function to find valid wordle solutions
// explores possible character combinations for empty slots
void words(std::string& current, const std::string& floating, const std::set<std::string>& dict, std::set<std::string>& results, int index, int empty);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here

    // initialize an empty set to store the valid word results found
    std::set<std::string> results;

    // create a mutable copy of the input pattern in this current string will be modified during the recursive search
    std::string current = in;

    // calculate the initial number of empty slots in the pattern
    int empty = countEmpty(current);

    // start the recursive search process by calling the words helper function
    // begins at index 0 with the current pattern, floating letters, dictionary, results set, and empty count
    words(current, floating, dict, results, 0, empty);

    // return the set containing all the valid words found
    return results;

}

// Define any helper functions here

// counts the number of hyphen characters in a string
int countEmpty(const std::string& in){

    // initialize a counter for empty slots to zero
    int count = 0;

    // iterate through each character of the input string
    for (size_t i = 0; i < in.length(); i++){

        // if the character at the current position is a hyphen
        if (in[i] == '-'){

            // increment the empty slot counter
            count++;
        }
    }

    // return the total count of empty slots found
    return count;
}

// recursive helper function to generate and validate word candidates
void words(std::string& current, const std::string& floating, const std::set<std::string>& dict, std::set<std::string>& results, int index, int empty){
 
    // check if the current index has reached the end of the word's length
    if (index == int(current.size())){

        if (!floating.empty()) {

             // if there are still required floating letters left, stop exploring this branch
             return;
        }


        // verify if the fully constructed current word exists in the provided dictionary
        if (dict.find(current) != dict.end()){

            // if the word is found in the dictionary, it is a valid solution
            // insert the valid word into the results set
            results.insert(current);
        }

        // whether the word was valid or not, this recursive path is complete
        // return to the previous call level
        return;
    }


    // the character at the current index is already fixed
    if (current[index] != '-'){

        // if the character is fixed, we don't need to guess
        // move to the next character position by making a recursive call
        // pass the same current word, floating letters, dict, results, incremented index, and the same empty count
        words(current, floating, dict, results, index + 1, empty);
        
        return;
    }

    // the character at the current index is an empty slot
    // we need to try filling it with possible letters
    // iterate through all lowercase letters from a to z
    for (char c = 'a'; c <= 'z'; c++){

        // tentatively place the current letter into the empty slot at index
        current[index] = c;

        // check if the chosen letter is one of the required floating letters
        size_t x = floating.find(c);

        // c is a required floating letter
        if (x != std::string::npos){

            std::string newFloating = floating.substr(0, x) + floating.substr(x + 1);

            words(current, newFloating, dict, results, index + 1, empty - 1);
        }
        // c is not a required floating letter
        else{

            if (empty > int(floating.size())){

                words(current, floating, dict, results, index + 1, empty - 1);
            }
        }
    }

    // backtracking
    // after trying all possible letters for the current empty slot at index
    // reset the character at current[index] back to - before returning
    current[index] = '-';
}

