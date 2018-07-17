//
// Created by subangkar on 7/14/18.
//

#ifndef IRGENERATOR_ASMPARSER_H
#define IRGENERATOR_ASMPARSER_H


#include <string>
#include <vector>
#include <cstring>
#include "Utils.h"
//#include "AsmBase.h"
#ifndef NEWLINE_ASM
#define NEWLINE_ASM string("\r\n")
#endif
using std::string;
using std::vector;

class ASMParser {
public:

	static void setOps(const vector<string> &cmdsSrc, string &op, string &dest, string &src) {
		unsigned long operands = cmdsSrc.size() > 2 ? 3 : cmdsSrc.size();
		switch (operands) {
			case 3:
				src = cmdsSrc[2];
			case 2:
				dest = cmdsSrc[1];
			case 1:
				op = cmdsSrc[0];
			default:;
		}
	}

	static string optimizedASM(const string &srcCode) {
		vector<string> srcInstSet = StringParser::str_tok(srcCode, NEWLINE_ASM);
		string destCode;
		for (int s = 0, d = 1; s < srcInstSet.size();) {
			string srcInst = srcInstSet[s];
			if (!srcInst.empty() && d < srcInstSet.size()) {
				string destInst = srcInstSet[d];
				string op1, dest1, src1;
				string op2, dest2, src2;
				vector<string> cmdsSrc = StringParser::str_tok(srcInst, "\t ,");
				setOps(cmdsSrc, op1, dest1, src1);
				vector<string> cmdsDest = StringParser::str_tok(destInst, "\t ,");
				setOps(cmdsDest, op2, dest2, src2);
				if (!destInst.empty() && op1 == "MOV" && op2 == "MOV") {
					if ((dest2 == src1 && src2 == dest1) || (dest1 == dest2 && src1 == src2)) {
						d++;
						continue;
					}
				} else if(op2.empty() && dest2.empty() && src2.empty()){
					d++;
					continue;
				}
			}
			destCode += srcInst + NEWLINE_ASM;
			s = d++;
		}
		return destCode;
	}
};

#endif //IRGENERATOR_ASMPARSER_H
