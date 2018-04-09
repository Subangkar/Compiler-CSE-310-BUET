//
// Created by SubangkarKr on 08-Apr-18.
//

#ifndef SYMBOLTABLE_LINKEDLIST_H
#define SYMBOLTABLE_LINKEDLIST_H


#include <cstdio>
#include <iostream>
#include <vector>

template<class T>
class Node {
public:
	explicit Node(const T &value) : data(value), next(NULL) {}

    ~Node() {}

    void SetValue(const T &value) { data = value; }

    const T &GetValue() const { return data; }

    // returns actual data pointer
    T* getData() { return &data;}

    void SetNext(Node<T> *next) { this->next = next; }

    Node<T> *GetNext() const { return next; }

    void Print() const { std::cout << data << std::endl; }

private:
//    Node();  // Prevent calling the default ctor

    T data;
    Node<T> *next;
};


template <class T>
class LinkedList {


    bool PushFront(const T& value);
    bool PushBack (const T& value);
    bool PopFront(T* value);
    bool PopBack (T* value);

    bool Insert(const T& value, const int& position);
    bool Insert(const T& value);

    bool Delete(const int& position);

public:
    LinkedList(): head(NULL), tail(NULL) {}
    ~LinkedList();

    int  length() const;
    void Print() const;

    bool insert(const T& value);

    bool remove(const T& value);

//    int  findPos(const int& value) const;
	int findPos(const T &value) const;
    bool search(const T& value) const;

	// returns the original data pointer if exists otherwise NULL
	T* getData(const T &value);
    bool Get (const int& position, T* value);

    std::vector<T> get() const;

private:
    Node<T>* head;
    Node<T>* tail;
};



#endif //SYMBOLTABLE_LINKEDLIST_H
