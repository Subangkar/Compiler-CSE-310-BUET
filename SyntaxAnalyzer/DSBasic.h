#pragma once
//
// Created by subangkar on 5/28/18.
//



#ifndef SYMBOLTABLE_CONSTANTS_H
#define SYMBOLTABLE_CONSTANTS_H


#define HASH_TABLE_MAXSIZE 59


#endif //SYMBOLTABLE_CONSTANTS_H



#ifndef SYNTAXANALYZER_DSBASIC_H
#define SYNTAXANALYZER_DSBASIC_H

#include <iostream>
#include <cmath>
#include <string>
#include <utility>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstring>

//using std::ostream;
//
//
//using std::vector;
//
//
//
////using std::cout;
////using std::endl;
//
//
//using std::string;
//
//
//
//using std::abs;

using namespace std;


#ifndef SYMBOLTABLE_TYPEDEFN_H
#define SYMBOLTABLE_TYPEDEFN_H


#ifndef EOF
#define EOF (-1)
#endif //EOF


typedef unsigned long long int64_p;


struct HASH_POS {
	int64_t loc = EOF;
	int64_t pos = EOF;

	HASH_POS() = default;

	HASH_POS(int64_t loc, int64_t pos) : loc(loc), pos(pos) {}

	bool isValid() {
		return loc != EOF && pos != EOF;
	}

	friend std::ostream &operator<<(std::ostream &os, const HASH_POS &pos) {
		os << "(" << pos.loc << "," << pos.pos << ")";
		return os;
	}
};

#endif //SYMBOLTABLE_TYPEDEFN_H


#ifndef SYMBOLTABLE_LINKEDLIST_H
#define SYMBOLTABLE_LINKEDLIST_H


template<class T>
class Node {
public:
	explicit Node(const T &value) : data(value), next(NULL) {}

	~Node() {}

	void SetValue(const T &value) { data = value; }

	const T &GetValue() const { return data; }

	// returns actual data pointer
	T *getData() { return &data; }

	void SetNext(Node<T> *next) { this->next = next; }

	Node<T> *GetNext() const { return next; }

	void Print() const { std::cout << data << std::endl; }

private:
//    Node();  // Prevent calling the default ctor

	T data;
	Node<T> *next;
};


template<class T>
class LinkedList {


	bool PushFront(const T &value);

	bool PushBack(const T &value);

	bool PopFront(T *value);

	bool PopBack(T *value);

	bool Insert(const T &value, const int &position);

	bool Insert(const T &value);

	bool Delete(const int &position);

public:
	LinkedList() : head(NULL), tail(NULL) {}

	~LinkedList();

	int length() const;

	void Print() const;

	bool insert(const T &value);

	bool remove(const T &value);

//    int  findPos(const int& value) const;
	int findPos(const T &value) const;

	bool search(const T &value) const;

	// returns the original data pointer if exists otherwise NULL
	T *getData(const T &value);

	bool Get(const int &position, T *value);

	std::vector<T> get() const;

private:
	Node<T> *head;
	Node<T> *tail;
};


#endif //SYMBOLTABLE_LINKEDLIST_H


#ifndef SYMBOLTABLE_LINKEDLIST_CPP
#define SYMBOLTABLE_LINKEDLIST_CPP


template<class T>
LinkedList<T>::~LinkedList() {
	Node<T> *current = head;

	while (current) {
		Node<T> *next = current->GetNext();
		delete (current);
		current = next;
	}

	head = tail = NULL;
}

// O(1)
template<class T>
bool LinkedList<T>::PushFront(const T &value) {
	Node<T> *element = new Node<T>(value);

	if (element) {
		if (!head) {
			// Special case, list is empty
			head = element;
			tail = element;
		} else {
			element->SetNext(head);
			head = element;
		}
		return true;
	} else {
		return false; // Error allocating memory
	}
}

// O(1)
template<class T>
bool LinkedList<T>::PushBack(const T &value) {
	Node<T> *element = new Node<T>(value);

	if (element) {
		if (!head) {
			// Special case, list is empty
			head = element;
			tail = element;
		} else {
			tail->SetNext(element);
			tail = element;
		}
		return true;
	} else {
		return false;
	}
}

// O(1)
template<class T>
bool LinkedList<T>::PopFront(T *value) {
	if (value && head) {
		*value = head->GetValue();

		Node<T> *new_head = head->GetNext();
		delete head;
		head = new_head;

		return false;
	} else
		return true; // Error: nullptr passed as parameter or list empty
}

// O(n)
template<class T>
bool LinkedList<T>::PopBack(T *value) {
	if (value && tail) {
		*value = tail->GetValue();

		// Special case: one element list
		if (head == tail) {
			delete head;
			head = NULL;
			tail = NULL;
		} else {
			Node<T> *new_tail;

			new_tail = head;

			while (new_tail->GetNext() != tail) // Skip elements till the element before tail
				new_tail = new_tail->GetNext();

			new_tail->SetNext(NULL);
			delete tail;
			tail = new_tail;
		}
		return false;
	} else
		return true; // Error: nullptr passed as parameter or list empty
}

// O(n)
template<class T>
void LinkedList<T>::Print() const {
	Node<T> *e;
	for (e = head; e; e = e->GetNext())
		e->Print();
}

// O(n)
template<class T>
int LinkedList<T>::length() const {
	Node<T> *e;
	int count = 0;

	for (e = head; e; e = e->GetNext()) ++count;

	return count;
}

// best O(1), avg O(n), wst O(n)
template<class T>
bool LinkedList<T>::Insert(const T &value, const int &position) {
	if (position < 0)
		return true;

	if (!position) // Special case: position = 0, insert into head
		return PushFront(value);

	Node<T> *element_before = head;

	for (int count = 0; count < position - 1; ++count) {
		element_before = element_before->GetNext();

		if (!element_before)
			return true; // Out of bounds
	}

	Node<T> *new_element = new Node<T>(value);
	if (!new_element)
		return true;

	new_element->SetNext(element_before->GetNext());
	element_before->SetNext(new_element);

	if (element_before == tail) // Special case: insert at the end
		tail = new_element;

	return false;
}

template<class T>
bool LinkedList<T>::Delete(const int &position) {
	if (position < 0)
		return false;

	if (!head)
		return false; // LinkedList empty

	Node<T> *target = head;

	if (!position) {// Special case: position = 0, delete head
		head = head->GetNext();
		delete target;

		return true;
	}

	Node<T> *element_before = head;
	for (int count = 0; count < position - 1; ++count) {
		element_before = element_before->GetNext();

		if (!element_before)
			return false; // Out of bounds
	}

	target = element_before->GetNext();

	if (!target)
		return false; // Out of bounds

	element_before->SetNext(target->GetNext());

	if (target == tail)
		tail = element_before;

	delete target;

	return true;
}

template<class T>
int LinkedList<T>::findPos(const T &value) const {
	int position = -1;

	Node<T> *e = head;

	for (int count = 0; e; e = e->GetNext(), ++count) {
		if (e->GetValue() == value) {
			position = count;
			break;
		}
	}

	return position;
}

template<class T>
bool LinkedList<T>::Get(const int &position, T *value) {
	if (position < 0 || !value)
		return true; // invalid input

	Node<T> *e = head;

	for (int count = 0; e && count < position; e = e->GetNext(), ++count);

	if (!e)
		return true;

	*value = e->GetValue();

	return false;
}

template<class T>
bool LinkedList<T>::Insert(const T &value) {
	return this->PushBack(value);
}

//template<class T>
//bool LinkedList<T>::Delete(const T &value) {
//    return Delete(findPos(value));
//}

template<class T>
bool LinkedList<T>::remove(const T &value) {
	return Delete(findPos(value));
}

template<class T>
bool LinkedList<T>::insert(const T &value) {

	if (search(value)) return false;

	return this->Insert(value);
}

template<class T>
bool LinkedList<T>::search(const T &value) const {
//    return findPos(value)!=-1;

	for (Node<T> *e = head; e; e = e->GetNext()) {
		if (e->GetValue() == value) {
			return true;
		}
	}

	return false;
}

template<class T>
std::vector<T> LinkedList<T>::get() const {

	std::vector<T> vc;

	for (Node<T> *e = head; e; e = e->GetNext()) {
		vc.push_back(e->GetValue());
	}

	return vc;
}

template<class T>
T *LinkedList<T>::getData(const T &value) {

	for (Node<T> *e = head; e; e = e->GetNext()) {
		if (e->GetValue() == value)
			return e->getData();
	}
	return NULL;
}

//template<class T>
//int LinkedList<T>::findPos(const T &value) const {
//    int position = -1;
//
//    Node<T>* e = head;
//    int c=0;
//    for(e=head;e->GetNext();c++){
//        if(e->GetValue()==value)
//        {
//            return c;
//        }
//    }
//
//    return position;
//}






#endif //SYMBOLTABLE_LINKEDLIST_CPP


#ifndef SYMBOLTABLE_HASHTABLE_H
#define SYMBOLTABLE_HASHTABLE_H



template<typename T>
class HashTable {
public:
	HashTable(); //default constructor
	explicit HashTable(int); //one parameter constructor
	HashTable(const HashTable &); //copy constructor
	virtual ~HashTable(); //destructor
	HashTable &operator=(const HashTable &); //assignment operator
	bool insert(const T &);

//	bool remove(const string&);
	bool remove(const T &);

//	bool search(const string&) const;
	bool search(const T &) const;

	int size() const; //return numOfItems
	int maxSize() const; //return arrSize
	double loadFactor() const;

	vector<T> get() const; //returns a vector of all the strings in the HashTable

	void printTable(FILE *printStream = stdout);

	// returns the original data pointer if exists otherwise NULL
	T *get(const T &key);

	int64_t getLOC(const T &key); // index of table

	int64_t getPOS(const T &key); // position in chain

	HASH_POS getHASHPOS(const T &key);

	bool isPrintEmptyListOn() const;

	void setPrintEmptyListOn(bool printEmptyListOn);


private:
	LinkedList<T> *arr;
	int arrSize;
	int numOfItems;

	int64_p hashFunc(const T &) const;

	void deepCopy(const HashTable &h);

	bool printEmptyListOn = false;
};

#endif //SYMBOLTABLE_HASHTABLE_H


#ifndef SYMBOLTABLE_HASHTABLE_CPP
#define SYMBOLTABLE_HASHTABLE_CPP


/*====================================================================
 *						PRIVATE MEMBER FUNCTIONS
 *====================================================================*/

template<typename T>
int64_p
HashTable<T>::hashFunc(const T &s) const //hash function (utilizes horner's method to prevent overflow on large strings)
{
	return s.hashValue() % arrSize;
}


template<typename T>
void HashTable<T>::deepCopy(const HashTable &h) {
	if (h.arr != NULL) {
		numOfItems = h.size();
		arrSize = h.maxSize();
		arr = new LinkedList<T>[arrSize];
		for (int i = 0; i < arrSize; i++)
			arr[i] = h.arr[i];
	}
}

template<typename T>
vector<T> HashTable<T>::get() const //returns a vector of all the strings in the hash table
{
	vector<T> v, tmp_v;
	for (int i = 0; i < maxSize(); i++) {
		tmp_v = arr[i].get();
		for (int j = 0; j < tmp_v.size(); j++)
			v.push_back(tmp_v[j]);
	}
	return v;
}

/*====================================================================
 *						PUBLIC MEMBER FUNCTIONS
 *====================================================================*/

template<typename T>
HashTable<T>::HashTable() //default constructor
{
	arrSize = HASH_TABLE_MAXSIZE;
	arr = new LinkedList<T>[arrSize];
	numOfItems = 0;
}

template<typename T>
HashTable<T>::HashTable(
		int n) //creates a hash table to store n items where the size of the array is the smallest prime number >= 2*n
{
	arrSize = n;
	arr = new LinkedList<T>[arrSize];
	numOfItems = 0;
}

template<typename T>
HashTable<T>::HashTable(const HashTable &h) //copy constructor
{
	deepCopy(h);
}

template<typename T>
HashTable<T>::~HashTable() //destructor
{
	delete[] arr;
}

template<typename T>
HashTable<T> &HashTable<T>::operator=(const HashTable <T> &h) //assignment operator
{
	if (this != &h) {
		if (h.arr != NULL)
			delete[] arr;
		deepCopy(h);
	}
	return *this;
}

//template <typename T>
//bool HashTable<T>::insert(const string& s) //inserts string s if it doesn't exist in the hash table and returns 1 if insertion successful, 0 otherwise
//{
//	int hash=hashFunc(s);
//	bool successOrFail=arr[hash].insert(s);
//	numOfItems++;
//	return successOrFail;
//}

//template <typename T>
//bool HashTable<T>::remove(const string& s) //removes string s if s exist in the hash table and returns 1 if removal successful, 0 otherwise
//{
//	int hash=hashFunc(s);
//	bool successOrFail=arr[hash].remove(s);
//	numOfItems--;
//	return successOrFail;
//}

//template <typename T>
//bool HashTable<T>::search(const string& s) const //returns 1 if s exist in the hash table, 0 otherwise
//{
//	int hash=hashFunc(s);
//	bool found=arr[hash].search(s);
//	return found;
//}

template<typename T>
int HashTable<T>::size() const //returns numOfItems
{
	return numOfItems;
}

template<typename T>
int HashTable<T>::maxSize() const //returns arrSize
{
	return arrSize;
}

template<typename T>
double HashTable<T>::loadFactor() const //returns the load factor of the hash table
{
	return (numOfItems * 1.0) / arrSize;
}

template<typename T>
bool HashTable<T>::insert(const T &s) {
	int64_p hash = hashFunc(s);
//
//	if (arr[hash].insert(s)) {
//		numOfItems++;
//		return HASH_POS(hash, arr[hash].length() - 1);
//	}
//	return HASH_POS(EOF, EOF);

	bool successOrFail = arr[hash].insert(s);
	if (successOrFail) numOfItems++;
	return successOrFail;
}

template<typename T>
bool HashTable<T>::remove(const T &s) {
	int hash = hashFunc(s);
	bool successOrFail = arr[hash].remove(s);
	numOfItems--;
	return successOrFail;
}

template<typename T>
bool HashTable<T>::search(const T &s) const {
	int hash = hashFunc(s);
	bool found = arr[hash].search(s);
	return found;
}

template<typename T>
void HashTable<T>::printTable(FILE *printStream) {
	for (int i = 0; i < arrSize; ++i) {

		if (printEmptyListOn || arr[i].length())
		{
//			cout << " " << i << " --> ";
//			cout << " " << std::setw(3) << std::setfill('0') << i << " --> ";
			fprintf(printStream, " %03d --> ", i);


			vector<T> vc = arr[i].get();

			for (int j = 0; j < vc.size(); j++) {
//				cout << vc[j];
//				fprintf(printStream,"%s",vc[j].printString().data());
//				fprintf(printStream, "%s", (char *) vc[j]);
//				cout << vc[j].printString();
			}

//			cout << endl;
			fprintf(printStream, "\n");

		}

	}
}

template<typename T>
T *HashTable<T>::get(const T &key) {
	int64_p hash = hashFunc(key);
	return arr[hash].getData(key);

}

template<typename T>
int64_t HashTable<T>::getLOC(const T &key) {
	return arr[hashFunc(key)].search(key) ? hashFunc(key) : EOF;
}

template<typename T>
int64_t HashTable<T>::getPOS(const T &key) {
	return arr[hashFunc(key)].findPos(key);
}

template<typename T>
HASH_POS HashTable<T>::getHASHPOS(const T &key) {
	return HASH_POS(getLOC(key), getPOS(key));
}

template<typename T>
bool HashTable<T>::isPrintEmptyListOn() const {
	return printEmptyListOn;
}

template<typename T>
void HashTable<T>::setPrintEmptyListOn(bool setprintEmptyListOn) {
	printEmptyListOn = setprintEmptyListOn;
}


#endif //SYMBOLTABLE_HASHTABLE_CPP



#endif //SYNTAXANALYZER_DSBASIC_H
