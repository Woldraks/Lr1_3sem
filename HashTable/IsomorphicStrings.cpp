#include "IsomorphicStrings.h"
#include <iostream>
using namespace std;

bool IsomorphicChecker::areIsomorphic(const string& s1, const string& s2) {
    if (s1.length() != s2.length()) {
        return false;
    }
    
   
    HashTable* map1 = CreateOpenAddressingTable(256);  
    HashTable* map2 = CreateOpenAddressingTable(256);  
    
    bool result = true;  
    
   
    for (size_t i = 0; i < s1.length(); i++) {
        char c1 = s1[i];  // Текущий символ из первой строки
        char c2 = s2[i];  // Текущий символ из второй строки
        
        // Преобразуем символы в строки для работы с хеш-таблицей
        string key1 = charToString(c1);  // Ключ для первой таблицы
        string key2 = charToString(c2);  // Ключ для второй таблицы
        string val1 = charToString(c2);  // Значение для первой таблицы (c1 -> c2)
        string val2 = charToString(c1);  // Значение для второй таблицы (c2 -> c1)
        
        // Проверяем отображение s1 -> s2
        string existing1 = OpenAddressingGet(map1, key1);
        
        if (existing1 != "NOT_FOUND") {
            // Для этого символа уже есть отображение
            // Проверяем, совпадает ли оно с текущим
            if (existing1 != val1) {
                // Несовпадение: символ c1 должен отображаться в c2,
                // но в таблице уже есть другое отображение
                result = false;
                break;
            }
        } else {
            // Новое отображение - добавляем в таблицу
            OpenAddressingInsert(map1, key1, val1);
        }
        
        // Проверяем обратное отображение s2 -> s1
        string existing2 = OpenAddressingGet(map2, key2);
        
        if (existing2 != "NOT_FOUND") {
            // Для этого символа уже есть отображение
            if (existing2 != val2) {
                // Несовпадение: символ c2 должен отображаться в c1,
                // но в таблице уже есть другое отображение
                result = false;
                break;
            }
        } else {
            // Новое отображение - добавляем в таблицу
            OpenAddressingInsert(map2, key2, val2);
        }
    }
    
    //Очистка памяти
    delete[] map1->table;
    delete map1;
    delete[] map2->table;
    delete map2;
    
    return result;
}

void IsomorphicChecker::printResult(const string& s1, const string& s2) {
    // Форматированный вывод результата
    cout << "  \"" << s1 << "\" и \"" << s2 << "\" → ";
    
    if (areIsomorphic(s1, s2)) {
        cout << "ИЗОМОРФНЫ " << endl;
    } else {
        cout << "НЕ ИЗОМОРФНЫ " << endl;
    }
}