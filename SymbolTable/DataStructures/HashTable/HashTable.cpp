//
// Created by SubangkarKr on 09-Apr-18.
//

#ifndef SYMBOLTABLE_HASHTABLE_CPP
#define SYMBOLTABLE_HASHTABLE_CPP




#include <iostream>
#include "HashTable.h"


using std::cout;
using std::endl;


/*====================================================================
 *						PRIVATE MEMBER FUNCTIONS
 *====================================================================*/

template<typename T>
int64_p HashTable<T>::hashFunc(const T &s) const //hash function (utilizes horner's method to prevent overflow on large strings)
{
	return s.hashValue() % arrSize;
}

template<typename T>
int HashTable<T>::getPrime(int n) const //return the smallest prime number >= 2*n
{
	int i = 2 * n;
	while (!isPrime(i))
		i++;
	return i;
}

template<typename T>
bool HashTable<T>::isPrime(int n) const //check whether n is prime, helper function for getPrime()
{
	bool isPrime = true;
	for (int count = 2; count < n && isPrime; count++)
		if (n % count == 0)
			isPrime = false;
	return isPrime;
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
	arrSize = getPrime(n);
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
HashTable<T> &HashTable<T>::operator=(const HashTable<T> &h) //assignment operator
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

	if (arr[hash].insert(s)) {
		numOfItems++;
		return true;
	}
	return false;

//	bool successOrFail = arr[hash].insert(s);
//	if (successOrFail) numOfItems++;
//	return successOrFail;
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
void HashTable<T>::printTable() {
	for (int i = 0; i < arrSize; ++i) {

		if (arr[i].length()) {
			cout << i << " :: ";

			vector<T> vc = arr[i].get();

			for (int j = 0; j < vc.size(); j++) {
				cout << vc[j];
			}

			cout << endl;

		}

	}
}

template<typename T>
T *HashTable<T>::get(const T &key) {
	int64_p hash = hashFunc(key);
	return arr[hash].getData(key);

}



#endif //SYMBOLTABLE_HASHTABLE_CPP
