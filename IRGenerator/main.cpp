#include <bits/stdc++.h>
#include "ASMParser.h"
#include "Utils.h"
//#define println cout<<
using namespace std;


int main()
{

	string str = "  PUSH BX                        \n"
	             "  PUSH CX                        \n"
	             "  PUSH DX                        \n"
	             "\n"
	             "  CMP AX, 0                      ; compare AX with 0\n"
	             "  JGE @START                     ; jump to label @START if AX>=0\n"
	             "";
	vector<string> toks = StringParser::str_tok(str,"\r\n");
	string opt = ASMParser::optimizedASM(str);
	cout << opt;
//	for (string s:toks){
//		cout << s <<endl;
//	}

	return 0;
}