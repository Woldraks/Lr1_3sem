#pragma once
#include <string>
using namespace std;

class XMLValidator {
public:
    static string recoverXML(const string& corrupted);
    static bool isValidXML(const string& xml);
};
