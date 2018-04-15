//
// Created by SubangkarKr on 07-Apr-18.
//

#include <cmath>
#include "1505015_SymbolInfo.h"

using std::abs;

const string &SymbolInfo::getName() const {
	return name;
}

void SymbolInfo::setName(const string &name) {
    SymbolInfo::name = name;
}

const string &SymbolInfo::getType() const {
    return type;
}

void SymbolInfo::setType(const string &type) {
    SymbolInfo::type = type;
}

SymbolInfo::SymbolInfo(const string &name, const string &type) : name(name), type(type) {}

SymbolInfo::SymbolInfo(const SymbolInfo &symbolInfo) {
    this->name = symbolInfo.name;
    this->type = symbolInfo.type;
}

bool SymbolInfo::operator==(const SymbolInfo &rhs) const {
//    return name == rhs.name && type == rhs.type;
    return name == rhs.name;
}

bool SymbolInfo::operator!=(const SymbolInfo &rhs) const {
    return !(rhs == *this);
}

SymbolInfo &SymbolInfo::operator=(const SymbolInfo &symbolInfo) {
    this->name = symbolInfo.name;
    this->type = symbolInfo.type;

    return *this;
}

int64_p SymbolInfo::hashValue() const {

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

std::ostream &operator<<(std::ostream &os, const SymbolInfo &info) {
	os << "< " << info.name << " : " << info.type << " > ";
	return os;
}

int64_p SymbolInfo::reverseDigits(int64_p n) const {
	int64_p revN = 0;
	while (n > 0) {
		revN = revN * 10 + n % 10;
		n /= 10;
	}
	return revN;
}

