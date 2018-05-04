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
		findAndReplaceAll(parsed, "\\n", "\n");
		findAndReplaceAll(parsed, "\\t", "\t");
		findAndReplaceAll(parsed, "\\\'", "\'");
		findAndReplaceAll(parsed, "\\\"", "\"");
		return parsed;
	}

	static string parse(const char *str) {
		return parse(string(str));
	}

	static char parseChar(const string str)
	{
		return static_cast<char>(str.length() > 2 ? '\0' : parse(str)[0]);
	}

	static string toUpperCase(string str)
	{
		std::locale loc;
		for (char &i : str)
			i = toupper(i,loc);
		return str;
	}
};


#endif //STRINGPARSER_STRINGPARSER_H


class StringUtils {
public:
	static int occCount(string str, char ch) {
		return static_cast<int>(std::count(str.begin(), str.end(), ch));
	}

	static int occCount(const string &dest, string src) {
		int occurrences = 0;
		string::size_type pos = 0;
		while ((pos = dest.find(src, pos)) != string::npos) {
			++occurrences;
			pos += src.length();
		}
		return occurrences;
	}

	static void replaceAll(string &source, string const &find, string const &replace) {
		for (string::size_type i = 0; (i = source.find(find, i)) != string::npos;) {
			source.replace(i, find.length(), replace);
			i += replace.length();
		}
	}

	// not checked
	static void replaceFirst(string &source, string const &find, string const &replace) {
		int i = static_cast<int>(source.find(find, 0));
		if (i != string::npos)
			source.replace(static_cast<unsigned long>(i), find.length(), replace);
	}

	// not checked
	static void replaceLast(string &source, string const &find, string const &replace) {
		int i = static_cast<int>(source.rfind(find));
		if (i != string::npos)
			source.replace(static_cast<unsigned long>(i), find.length(), replace);
	}
};


#endif //LEXICALANALYZER_UTILS_H
