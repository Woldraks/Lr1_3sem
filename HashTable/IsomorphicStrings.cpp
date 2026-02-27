#include "IsomorphicStrings.h"
#include <iostream>
#include <unordered_map>
using namespace std;

bool IsomorphicChecker::areIsomorphic(const string& s1, const string& s2) {
    if (s1.length() != s2.length()) {
        return false;
    }
    
    unordered_map<char, char> map1;
    unordered_map<char, char> map2;
    
    for (size_t i = 0; i < s1.length(); i++) {
        char c1 = s1[i];
        char c2 = s2[i];
        
        if (map1.find(c1) != map1.end()) {
            if (map1[c1] != c2) {
                return false;
            }
        } else {
            map1[c1] = c2;
        }
        
        if (map2.find(c2) != map2.end()) {
            if (map2[c2] != c1) {
                return false;
            }
        } else {
            map2[c2] = c1;
        }
    }
    
    return true;
}

void IsomorphicChecker::printResult(const string& s1, const string& s2) {
    cout << "Строки \"" << s1 << "\" и \"" << s2 << "\" ";
    if (areIsomorphic(s1, s2)) {
        cout << "ИЗОМОРФНЫ" << endl;
    } else {
        cout << "НЕ ИЗОМОРФНЫ" << endl;
    }
}
