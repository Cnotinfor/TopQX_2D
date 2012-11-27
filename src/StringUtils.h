#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <list>

using namespace std;

class StringUtils
{
public:
	// Get a translated string
	static string getStringValue(string file_name, string stringkey);

	// Split a string in many others
	static list<string> split(string stringToSplit, const char* delimiter);
//private:
	//static string specialCharsConvertion(string strIn);
};

#endif // STRINGUTILS_H