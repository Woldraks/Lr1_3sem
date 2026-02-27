#include "XMLValidator.h"
#include "Stack.h"
#include <iostream>
#include <string>
using namespace std;

// Проверка, что строка состоит только из строчных латинских букв
bool isValidTagName(const string& tag) {
    if (tag.empty()) return false;
    for (size_t i = 0; i < tag.length(); i++) {
        if (tag[i] < 'a' || tag[i] > 'z') return false;
    }
    return true;
}

bool XMLValidator::isValidXML(const string& xml) {
    // Правило 1: Пустая строка - корректный XML
    if (xml.empty()) return true;
    
    Stack* stack = CreateStack();
    string currentTag = "";
    bool inTag = false;
    bool isClosing = false;
    int i = 0;
    
    while (i < xml.length()) {
        char c = xml[i];
        
        if (c == '<') {
            // Начало тега
            if (inTag) {
                // Не может быть два тега подряд без закрытия
                ClearStack(stack);
                delete stack;
                return false;
            }
            inTag = true;
            currentTag = "";
            isClosing = false;
            i++;
            
            // Проверяем, может это закрывающий тег </
            if (i < xml.length() && xml[i] == '/') {
                isClosing = true;
                i++;
            }
        }
        else if (c == '>') {
            // Конец тега
            if (!inTag) {
                ClearStack(stack);
                delete stack;
                return false;
            }
            
            // Проверяем имя тега
            if (!isValidTagName(currentTag)) {
                ClearStack(stack);
                delete stack;
                return false;
            }
            
            if (isClosing) {
                // Правило 3: Закрывающий тег должен соответствовать открывающему
                if (isEmpty(stack)) {
                    ClearStack(stack);
                    delete stack;
                    return false;
                }
                
                string lastTag = Pop(stack);
                if (lastTag != currentTag) {
                    ClearStack(stack);
                    delete stack;
                    return false;
                }
            } else {
                // Правило 3: Открывающий тег
                Push(stack, CreateStackElement(currentTag));
            }
            
            inTag = false;
            i++;
        }
        else if (isalpha(c) && c >= 'a' && c <= 'z') {
            // Буква внутри тега
            if (!inTag) {
                // Символы вне тегов не допускаются в нашей модели
                // (кроме случая, когда это часть конкатенации)
                ClearStack(stack);
                delete stack;
                return false;
            }
            currentTag += c;
            i++;
        }
        else {
            // Недопустимый символ
            ClearStack(stack);
            delete stack;
            return false;
        }
    }
    
    // Если остались незакрытые теги - ошибка
    // Также не должны оставаться внутри тега
    bool result = (isEmpty(stack) && !inTag);
    ClearStack(stack);
    delete stack;
    return result;
}

string XMLValidator::recoverXML(const string& corrupted) {
    if (corrupted.length() < 7 || corrupted.length() > 1000) {
        return "";
    }
    
    // Сначала проверим, может строка уже корректна?
    if (isValidXML(corrupted)) {
        return corrupted;
    }
    
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    string symbols = "<>/";
    
    // Пробуем восстановить, заменяя каждый символ
    for (size_t i = 0; i < corrupted.length(); i++) {
        char original = corrupted[i];
        
        // Пробуем заменить на буквы
        if (isalpha(original)) {
            for (char c : alphabet) {
                if (c == original) continue;
                
                string candidate = corrupted;
                candidate[i] = c;
                
                if (isValidXML(candidate)) {
                    return candidate;
                }
            }
        }
        
        // Пробуем заменить на символы < > /
        for (char c : symbols) {
            if (c == original) continue;
            
            string candidate = corrupted;
            candidate[i] = c;
            
            if (isValidXML(candidate)) {
                return candidate;
            }
        }
    }
    
    return "";
}
