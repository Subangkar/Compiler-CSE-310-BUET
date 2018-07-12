//
// Created by subangkar on 5/2/18.
//

#ifndef LEXICALANALYZER_UTILS_H
#define LEXICALANALYZER_UTILS_H
//
// Created by subangkar on 5/2/18.
//

#ifndef STRINGPARSER_STRINGPARSER_H
#define STRINGPARSER_STRINGPARSER_H


#include <string>
#include <algorithm>
#include <locale>

using std::string;

class StringParser {
//	static const string nwline = "\n", tab = "\t", slash = "\\";

	static void findAndReplaceAll(string &source, string const &find, string const &replace) {
		for (string::size_type i = 0; (i = source.find(find, i)) != string::npos;) {
			source.replace(i, find.length(), replace);
			i += replace.length();
		}
	}

public:
	static string parse(const string &str) {
		string parsed = str;
//		findAndReplaceAll(parsed, "\\n", "\n");
		findAndReplaceAll(parsed, "\\n", "\n");
		findAndReplaceAll(parsed, "\\t", "\t");
		findAndReplaceAll(parsed, "\\\'", "\'");
		findAndReplaceAll(parsed, "\\\"", "\"");
		return parsed;
	}

	static string parse(const char *str) {
		return parse(string(str));
	}

	static char parseChar(const string &str) {
		return static_cast<char>(str.length() > 2 ? '\0' : parse(str)[0]);
	}

	static string toUpperCase(string str) {
		std::locale loc;
		for (char &i : str)
			i = toupper(i, loc);
		return str;
	}

	static int toInteger(const string& str){
		return atoi(str.data());
	}
};


#endif //STRINGPARSER_STRINGPARSER_H


class StringUtils {
public:
	static int occCount(string str, char ch) {
		return static_cast<int>(std::count(str.begin(), str.end(), ch));
	}

	static int occCount(const string &dest, const string &src) {
		int occurrences = 0;
		string::size_type pos = 0;
		while ((pos = dest.find(src, pos)) != string::npos) {
			++occurrences;
			pos += src.length();
		}
		return occurrences;
	}

	static bool replaceAll(string &source, string const &find, string const &replace) {
		if (source.find(find, 0) == string::npos) return false;
		for (string::size_type i = 0; (i = source.find(find, i)) != string::npos;) {
			source.replace(i, find.length(), replace);
			i += replace.length();
		}
		return true;
	}

	// not checked
	static void replaceFirst(string &source, string const &find, string const &replace) {
		auto i = static_cast<int>(source.find(find, 0));
		if (i != string::npos)
			source.replace(static_cast<unsigned long>(i), find.length(), replace);
	}

	// not checked
	static void replaceLast(string &source, string const &find, string const &replace) {
		auto i = static_cast<int>(source.rfind(find));
		if (i != string::npos)
			source.replace(static_cast<unsigned long>(i), find.length(), replace);
	}

	static bool isAlpha(char c){
		return (c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z')||c=='_';
	}

	static bool isNumber(const string& s){
		for (char it : s) {
			if(it <'0' || it >'9') return false;
		}
		return true;
	}
};


class CodeParser {
public:

	static string indent(const string &fileName)//Autoindent a file
	{

		int pos = 0, count = 0, j = 0;
		string ch;
		stringstream file(fileName);
		pos = StringUtils::occCount(fileName, "\n");
		vector<string> hold(static_cast<unsigned long>(pos + 1));//holds the actual content of the line.
		vector<int> bracketcount(static_cast<unsigned long>(pos + 1));//keeps track of number of nested brackets

		while (getline(file, hold[count])) {
			count++;
		}
		int endline = count;
		int p = 0;
		while (p <= endline) {
			bracketcount[p] = 0;
			p++;
		}
		count = 0;
		int len = 0, i = 0, current = 0;
		while (count < endline) {
			len = static_cast<int>(hold[count].length());
			i = 0;
			while (i <= len) {
				if (hold[count][i] == '{') {
					current = count;
					while (current < endline) {
						current++;
						bracketcount[current]++;
					}
				}
				if (hold[count][i] == '}') {
					current = count;
					while (current < endline) {
						bracketcount[current]--;
						current++;
					}
				}
				i++;
			}
			count++;
		}


		stringstream file2;
		// I'm probably just being overly cautious.
		vector<string> indents(static_cast<unsigned long>(pos + 1));
		p = 0;
		while (p < endline) {
			count = 0;
			while (hold[p][count] == '\t')//check if there were already tabs in the file
			{
				count++;
			}
			while (bracketcount[p] > 0)//tells indents[p] how many indents should be in a line
			{
				bracketcount[p]--;
				indents[p] += "\t";
			}
			if (count == 0)//if the line wasn't previously indented in the code
			{
				file2 << indents[p] << hold[p] << endl;
			} else if (count > 0)//If there were already indents in the code then set them to the right amount
			{
				file2 << indents[p];
				len = static_cast<int>(hold[p].length());
				i = 0;
				while (i < len - count) {
					file2 << hold[p][i + count];
					i++;
				}
				file2 << endl;
			}
			p++;
		}
		return file2.str();
	}

	static string formatCCode(const string &code) {
		string formattedCode = code;

		while (StringUtils::replaceAll(formattedCode, " ;", ";"));
		StringUtils::replaceAll(formattedCode, ";", ";\n");
		StringUtils::replaceAll(formattedCode, ";\n\n", ";\n");


		while (StringUtils::replaceAll(formattedCode, "( ", "("));
		while (StringUtils::replaceAll(formattedCode, " (", "("));
		while (StringUtils::replaceAll(formattedCode, ") ", ")"));
		while (StringUtils::replaceAll(formattedCode, " )", ")"));

		while (StringUtils::replaceAll(formattedCode, "{ ", "{"));
		while (StringUtils::replaceAll(formattedCode, " {", "{"));
		while (StringUtils::replaceAll(formattedCode, "} ", "}"));
		while (StringUtils::replaceAll(formattedCode, " }", "}"));


		StringUtils::replaceAll(formattedCode, "{", "\n{\n");

		StringUtils::replaceAll(formattedCode, "}", "\n}\n");
		while (StringUtils::replaceAll(formattedCode, "\n\n}", "\n}"));
		while (StringUtils::replaceAll(formattedCode, "}\n\n", "}\n"));

		while (StringUtils::replaceAll(formattedCode, "  = ", " = "));
		while (StringUtils::replaceAll(formattedCode, " =  ", " = "));

		while (StringUtils::replaceAll(formattedCode, "  == ", " == "));
		while (StringUtils::replaceAll(formattedCode, " ==  ", " == "));


		StringUtils::replaceAll(formattedCode, "(", " ( ");
		StringUtils::replaceAll(formattedCode, ")", " ) ");

		while (StringUtils::replaceAll(formattedCode, " ++", "++"));
		while (StringUtils::replaceAll(formattedCode, " --", "--"));

		while (StringUtils::replaceAll(formattedCode, "\n ", "\n"));
		while (StringUtils::replaceAll(formattedCode, " \n", "\n"));
		while (StringUtils::replaceAll(formattedCode, "  ", " "));

		if (formattedCode[0] == ' ') StringUtils::replaceFirst(formattedCode," ","");
//		return indent(formattedCode);
		return formattedCode;
	}
};

#endif //LEXICALANALYZER_UTILS_H
