//
// Created by SubangkarKr on 09-Apr-18.
//

#ifndef SYMBOLTABLE_HASHTABLE_H
#define SYMBOLTABLE_HASHTABLE_H





#pragma once

#include <vector>

#include "../../Data/Const.h"
#include "../LinkedList/LinkedList.h"
#include "../LinkedList/LinkedList.cpp"
#include "../../DataTypes/TypeDefn.h"


using std::vector;



template <typename T>
class HashTable
{
public:
	HashTable(); //default constructor
	HashTable(int); //one parameter constructor
	HashTable(const HashTable&); //copy constructor
	virtual ~HashTable(); //destructor
	HashTable& operator=(const HashTable&); //assignment operator
	bool insert(const T&);
//	bool remove(const string&);
	bool remove(const T&);
//	bool search(const string&) const;
	bool search(const T&) const;
	int size() const; //return numOfItems
	int maxSize() const; //return arrSize
	double loadFactor() const;
	vector<T> get() const; //returns a vector of all the strings in the HashTable

	void printTable();

	// returns the original data pointer if exists otherwise NULL
	T* get(const T&key);
private:
	LinkedList<T>* arr;
	int arrSize;
	int numOfItems;
	int64_p hashFunc(const T&) const;
	int getPrime(int) const;
	bool isPrime(int) const;
	void deepCopy(const HashTable& h);
};

#endif //SYMBOLTABLE_HASHTABLE_H
