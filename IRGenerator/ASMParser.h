//
// Created by subangkar on 7/14/18.
//

#ifndef IRGENERATOR_ASMPARSER_H
#define IRGENERATOR_ASMPARSER_H


#include <string>
#include <vector>
#include <cstring>

using std::string;
using std::vector;

class ASMParser
{
public:
	static string optimize(const string& src){
//		sstream
		vector<string> srcInst;
		char temp[15];
		while(strtok(temp,"\n")){
			srcInst.emplace_back(temp);
		}
//		getline(,)
		return string(temp);
	}
};

#endif //IRGENERATOR_ASMPARSER_H
