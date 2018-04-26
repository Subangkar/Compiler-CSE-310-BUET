//
// Created by SubangkarKr on 07-Apr-18.
//

#ifndef SYMBOLTABLE_SYMBOLINFO_H
#define SYMBOLTABLE_SYMBOLINFO_H



#include <string>
#include <ostream>
#include "../TypeDefn.h"

using std::string;



class SymbolInfo {
	string name;
	string type;


public:
	const string &getName() const;

	explicit SymbolInfo(const string &name, const string &type = "");

	void setName(const string &name);

	const string &getType() const;

	void setType(const string &type);

	SymbolInfo(const SymbolInfo &symbolInfo);

	SymbolInfo &operator=(const SymbolInfo &symbolInfo);

	bool operator==(const SymbolInfo &rhs) const;

	bool operator!=(const SymbolInfo &rhs) const;

	int64_p hashValue() const;

	friend std::ostream &operator<<(std::ostream &os, const SymbolInfo &info);


private:

	int64_p reverseDigits(int64_p n) const;

};


#endif //SYMBOLTABLE_SYMBOLINFO_H
