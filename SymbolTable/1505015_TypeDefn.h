//
// Created by SubangkarKr on 09-Apr-18.
//

#ifndef SYMBOLTABLE_TYPEDEFN_H
#define SYMBOLTABLE_TYPEDEFN_H

#include <ostream>

#ifndef EOF
#define EOF -1
#endif //EOF


typedef unsigned long long int64_p;
typedef long long int64;


struct HASH_POS {
	int64_t loc = EOF;
	int64_t pos = EOF;

	HASH_POS() {}

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


