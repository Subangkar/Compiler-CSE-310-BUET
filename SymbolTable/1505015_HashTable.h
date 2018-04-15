//
// Created by SubangkarKr on 09-Apr-18.
//

#ifndef SYMBOLTABLE_HASHTABLE_H
#define SYMBOLTABLE_HASHTABLE_H





#pragma once

#include <vector>

#include "1505015_Const.h"
#include "1505015_LinkedList.h"
#include "1505015_LinkedList.cpp"
#include "1505015_TypeDefn.h"


using std::vector;



template <typename T>
class HashTable
{
public:
	HashTable(); //default constructor
	explicit HashTable(int); //one parameter constructor
	HashTable(const HashTable&); //copy constructor
	virtual ~HashTable(); //destructor
	HashTable& operator=(const HashTable&); //assignment operator
	bool insert(const T &);
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

	int64_t getLOC(const T &key); // index of table

	int64_t getPOS(const T &key); // position in chain

	HASH_POS getHASHPOS(const T& key);
private:
	LinkedList<T>* arr;
	int arrSize;
	int numOfItems;
	int64_p hashFunc(const T&) const;

	void deepCopy(const HashTable& h);
};

#endif //SYMBOLTABLE_HASHTABLE_H
