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


#include <vector>
#include <string>
#include <algorithm>
#include <locale>

using std::string;
using std::vector;
using std::stringstream;

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

	static int toInteger(const string &str) {
		return atoi(str.data());
	}
	// ============================================================================
	//    Author: K Perkins
	//    Date:   Jun 13, 2014
	//    Taken From: http://programmingnotes.org/
	//    File:  Strtok.cpp
	//    Description: Demonstrates the use of a custom "strtok" function which
	//      splits a string into individual tokens according to delimiters.
	// ============================================================================

	/**
	* FUNCTION: str_tok
	* USE: Splits a string into individual tokens and saves them into a vector.
	* @param str: A std::string to be broken up into smaller std::strings (tokens).
	* @param delimiter: A std::string containing the delimiter characters.
	* @return: A vector containing all the found tokens in the string.
	*/
	// http://programmingnotes.org/
	static vector<string> str_tok(const string &str, const string &delimiters) {
		unsigned prev = 0;
		unsigned long currentPos = 0;
		vector <string> tokens;

		// loop thru string until we reach the end
		while ((currentPos = str.find_first_of(delimiters, prev)) != string::npos) {
			if (currentPos > prev) {
				tokens.push_back(str.substr(prev, currentPos - prev));
			}
			prev = static_cast<unsigned int>(currentPos + 1);
		}
		// if characters are remaining, save to vector
		if (prev < str.length()) {
			tokens.push_back(str.substr(prev, string::npos));
		}
		return tokens;

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

	static bool isAlpha(char c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
	}

	static bool isNumber(const string &s) {
		for (char it : s) {
			if (it < '0' || it > '9') return false;
		}
		return true;
	}
};

#endif //LEXICALANALYZER_UTILS_H
