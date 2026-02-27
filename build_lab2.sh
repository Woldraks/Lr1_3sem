#!/bin/bash


g++ -std=c++11 \
    Array/Array.cpp \
    ForwardList/SinglyLinkedList.cpp \
    LinkedList/DoublyLinkedList.cpp \
    Queue/Queue.cpp \
    Stack/Stack.cpp \
    Tree/Tree.cpp \
    Stack/XMLValidator.cpp \
    HashSet/HashSet.cpp \
    SetPartition/SetPartition.cpp \
    Array/Subarrays/Subarrays.cpp \
    Tree/BSTHeight/BSTHeight.cpp \
    HashTable/HashTable.cpp \
    HashTable/IsomorphicStrings.cpp \
    LRUCache/LRUCache.cpp \
    main_lab2.cpp \
    -o lab2

if [ $? -eq 0 ]; then
    echo "Компиляция успешна! Запуск программы..."
    echo "========================================"
    ./lab2
else
    echo "Ошибка компиляции!"
fi
