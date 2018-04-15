#include <iostream>

#include "1505015_ScopeTable.h"


#include "1505015_SymbolTable.h"

using namespace std;


void symbolTableTest() {
	int size = 0;
	cin >> size;
//	cout <<size<<endl;
	SymbolTable *symbolTable = new SymbolTable(size);
	string op, sym, typ;
//	symbolTable->enterScope();
	while (cin >> op) {
		if (op == "I") {
			cin >> sym >> typ;
			cout << endl << op << " " << sym << " " << typ << endl << endl;
			symbolTable->insert(sym, typ);
		} else if (op == "D") {
			cin >> sym;
			cout << endl << op << " " << sym << " " << typ << endl << endl;
			symbolTable->remove(sym);
		} else if (op == "L") {
			cin >> sym;
			cout << endl << op << " " << sym << " " << typ << endl << endl;
			symbolTable->lookUp(sym);
		} else if (op == "S") {
			cout << endl << op << " " << sym << " " << typ << endl << endl;
			symbolTable->enterScope();
		} else if (op == "E") {
			cout << endl << op << " " << sym << " " << typ << endl << endl;
			symbolTable->exitScope();
		} else if (op == "P") {
			cin >> sym;
			cout << endl << op << " " << sym << " " << typ << endl << endl;
			if (sym == "A") symbolTable->printAllScope();
			else if (sym == "C") symbolTable->printCurrentScope();
		}

		sym="";
		typ="";
	}
//	symbolTable->exitScope();
}

int main() {
//    std::cout << "Hello, World!" << std::endl;

//    ScopeTable scopeTable;

//	scopeTableTest();

	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	symbolTableTest();


	return 0;
}