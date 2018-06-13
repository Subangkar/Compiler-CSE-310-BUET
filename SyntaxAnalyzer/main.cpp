#include <iostream>
#include <sstream>
#include "DataStructure.h"
#include "DataStructure.h"
#include "Utils.h"


string indent(string fileName)//Autoindent a file
{

	int pos = 0, count =0, j=0;
	string ch;
	stringstream file(fileName);

	pos = StringUtils::occCount(fileName,"\n");
	vector<string> hold(static_cast<unsigned long>(pos + 1));//holds the actual content of the line.
	vector<int> bracketcount(static_cast<unsigned long>(pos + 1));//keeps track of number of nested brackets


	while(getline(file,hold[count]))
	{
		count++;
	}
	int endline = count;
	int p=0;
	while(p<=endline)
	{
		bracketcount[p]=0;
		p++;
	}
	count = 0;
	int len=0,i = 0, current=0;
	while(count<endline)
	{
		len = static_cast<int>(hold[count].length());
		i=0;
		while(i<=len)
		{
			if(hold[count][i] == '{')
			{
				current=count;
				while(current <endline)
				{
					current++;
					bracketcount[current]++;
				}
			}
			if(hold[count][i] == '}')
			{
				current=count;
				while(current <endline)
				{
					bracketcount[current]--;
					current++;
				}
			}
			i++;
		}
		count++;
	}
//	file.close();



	stringstream file2;
	// I'm probably just being overly cautious.
	vector<string> indents(static_cast<unsigned long>(pos + 1));
	p=0;
	while(p<endline)
	{
		count=0;
		while(hold[p][count]=='\t')//check if there were already tabs in the file
		{
			count++;
		}
		while(bracketcount[p]>0)//tells indents[p] how many indents should be in a line
		{
			bracketcount[p]--;
			indents[p] +="\t";
		}
		if(count==0)//if the line wasn't previously indented in the code
		{
			file2<<indents[p]<<hold[p]<<endl;
		}
		else if(count>0)//If there were already indents in the code then set them to the right amount
		{
			file2<<indents[p];
			len = static_cast<int>(hold[p].length());
			i=0;
			while(i<len-count)
			{
				file2<<hold[p][i+count];
				i++;
			}
			file2<<endl;
		}
		p++;
	}
	return file2.str();
}


SymbolTable st;
int main() {

//	string code = "int main(){int x ,y ,z ;y =8 ;int d ;d =x +y ; z =(x ==y ) ; } ";
//	StringUtils::replaceAll(code,";",";\n");
//	StringUtils::replaceAll(code,";\n\n",";\n");
//	StringUtils::replaceAll(code,"\n ","\n");
//	StringUtils::replaceAll(code,"{","\n{\n");
//	StringUtils::replaceAll(code,"}","\n}");
//	StringUtils::replaceAll(code,"\n\n}","\n}");
////	StringUtils::replaceAll(code,"}","\n}");
//	cout << indent(code)<<endl;
//
//	st.insert(SymbolInfo("oot","FUNC"));
//	st.insert(SymbolInfo("oot","FUNC"));
//
//	st.printAllScope();

	cout << atof("2.5E.4")<<endl;

	return 0;
}