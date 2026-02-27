#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "HashSet/HashSet.h"

using namespace std;

// Разбиваем строку на токены
vector<string> split(const string& str) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    string filename;
    string query;
    
    // Парсим аргументы командной строки
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }
    
    if (filename.empty()) {
        cout << "Использование: " << argv[0] << " --file <filename> [--query <command>]" << endl;
        return 1;
    }
    
    // Если есть query, добавляем его в файл
    if (!query.empty()) {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << query << endl;
            file.close();
            cout << "Команда добавлена: " << query << endl;
        }
    }
    
    // Создаем множество
    HashSet* set = CreateHashSet(8);
    
    // Читаем и выполняем команды из файла
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл: " << filename << endl;
        DestroyHashSet(set);
        return 1;
    }
    
    string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;
        
        vector<string> tokens = split(line);
        if (tokens.size() < 2) {
            cout << "Ошибка в строке " << lineNum << ": недостаточно аргументов" << endl;
            continue;
        }
        
        string command = tokens[0];
        string key = tokens[1];
        
        if (command == "SETADD") {
            SetAdd(set, key);
            cout << "SETADD " << key << " -> добавлен" << endl;
        }
        else if (command == "SETDEL") {
            SetDel(set, key);
            cout << "SETDEL " << key << " -> удален" << endl;
        }
        else if (command == "SET_AT") {
            bool exists = SetAt(set, key);
            cout << "SET_AT " << key << " -> " << (exists ? "true" : "false") << endl;
        }
        else {
            cout << "Неизвестная команда: " << command << endl;
        }
    }
    
    file.close();
    
    // Выводим финальное состояние множества
    cout << "\nФинальное состояние множества: ";
    SetPrint(set);
    
    DestroyHashSet(set);
    return 0;
}
