#include <iostream>
#include <string>
#include <cmath>

#include "DataStructures/ScopeTable/ScopeTable.h"



#include "DataTypes/SymbolInfo/SymbolInfo.h"
#include "DataStructures/HashTable/HashTable.h"
#include "DataStructures/HashTable/HashTable.cpp"
#include "DataStructures/LinkedList/LinkedList.h"
#include "DataStructures/LinkedList/LinkedList.cpp"

using namespace std;



class A
{


	string name;


	int64_p reverseDigits( int64_p n ) const {
		int64_p revN = 0;
		while (n > 0) {
			revN = revN * 10 + n % 10;
			n /= 10;
		}
		return revN;
	}

public:
	friend ostream &operator<<(ostream &os, const A &a) {
		os << "<" << a.name << "> ";
		return os;
	}

	bool operator==(const A &rhs) const {
		return name == rhs.name;
	}

	bool operator!=(const A &rhs) const {
		return !(rhs == *this);
	}

	A(string s) : name(s){};

	int64_p hashValue() const
	{
		string key = this->name;

		int64_p value = 0;

		for (int i = 0; i < key.length(); i++) {
			if (i % 2 == 0) {
				value += abs( reverseDigits( key[i] ) ) * (int64_p) pow( 19 , i );
			} else {
				value += abs( (key[i]) * (int) pow( 23 , i ) );
			}
		}

		return value;
	}
};




void string_lltest()
{
    LinkedList<string> ll;

    string s = "abc";
    ll.insert(s);

    s="def";
    ll.insert(s);

    if(ll.search("de")){
        cout<<"Found de"<<endl;
    }
    if(ll.search("abc")){
        cout<<"Found abc"<<endl;
    }

    ll.remove("def");
    if(ll.search("abc")){
        cout<<"Found abc"<<endl;
    }
    ll.remove("der");
    ll.remove("abc");
    if(ll.search("abc")){
        cout<<"Found abc"<<endl;
    }

    ll.Print();

    ll.remove("abc");
    ll.remove("de");
    ll.search("abc");

    ll.insert("abc");
    if(ll.search("abc")){
        cout<<"Found abc"<<endl;
    }

}

void string_httest()
{
	HashTable<A> hashTable;



	hashTable.insert(A("av"));
	hashTable.insert(A("asdhd"));
	hashTable.insert(A("ave"));
	hashTable.insert(A("av"));
	hashTable.insert(A("avd"));

	cout<<hashTable.size()<<endl;


	cout<<hashTable.get().size()<<endl;

	hashTable.printTable();


	hashTable.insert(A("dv"));
	hashTable.insert(A("pc"));
	hashTable.remove(A("ave"));
	hashTable.insert(A("ave"));
	hashTable.insert(A("asad"));

	cout<<hashTable.size()<<endl;


	cout<<hashTable.get().size()<<endl;

	hashTable.printTable();


}

void symInfo_httest()
{
	HashTable<SymbolInfo> ht;

	SymbolInfo sm = SymbolInfo("name","string");
	ht.insert(sm);
	sm = SymbolInfo("name");
	ht.insert(sm);
	ht.remove(sm);
	ht.insert(SymbolInfo("name","array"));

	SymbolInfo *s = ht.get(SymbolInfo("name"));

	//ht.printTable();
//	string x = s->getName();
	cout<<*s<<endl;
}

void scopeTableTest()
{
	ScopeTable st;

	st.Insert("x","int");
	st.Insert("name","char*");

	cout<<*st.LookUp("x")<<endl;
	if(st.LookUp("y"))
	{
		cout<<*st.LookUp("y")<<endl;
	}
	st.Insert("y","int");
	if(st.LookUp("y"))
	{
		cout<<*st.LookUp("y")<<endl;
	}
	st.Insert("x","char");
	if(st.LookUp("x"))
	{
		cout<<*st.LookUp("x")<<endl;
	}
	st.Delete("x");
	if(st.LookUp("x"))
	{
		cout<<*st.LookUp("x")<<endl;
	}
	st.Insert("x","array");
	if(st.LookUp("x"))
	{
		cout<<*st.LookUp("x")<<endl;
	}
}


int main() {
//    std::cout << "Hello, World!" << std::endl;

//    ScopeTable scopeTable;

	scopeTableTest();


    return 0;
}