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
		findAndReplaceAll(parsed, "\\n", "\n");
		findAndReplaceAll(parsed, "\\t", "\t");
//		findAndReplaceAll(parsed, "\'", "'");
		return parsed;
	}

	static string parse(const char *str) {
		return parse(string(str));
	}
};



#endif //STRINGPARSER_STRINGPARSER_H


class StringUtils
{
public:
	static int occCount(string str,char ch)
	{
		return static_cast<int>(std::count(str.begin(), str.end(), ch));
	}

	static int occCount(string dest,string src)
	{
		int occurrences = 0;
		string::size_type pos = 0;
		while ((pos = dest.find(src, pos )) != std::string::npos) {
			++occurrences;
			pos += src.length();
		}
		return occurrences;
	}
};


#endif //LEXICALANALYZER_UTILS_H
