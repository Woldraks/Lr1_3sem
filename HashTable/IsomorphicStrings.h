#pragma once
#include <string>
#include "HashTable.h" 
using namespace std;


class IsomorphicChecker {
private:
    
    static inline string charToString(char c) {
        return string(1, c);
    }
    
public:
    
    static bool areIsomorphic(const string& s1, const string& s2);

    static void printResult(const string& s1, const string& s2);
 
    static void runTests();
};