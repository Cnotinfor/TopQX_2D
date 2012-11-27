#include "Stdafx.h"
#include "StringUtils.h"
#include "FileHandler.h"

//string StringUtils::specialCharsConvertion(string strIn)
//{
//#if (defined(_WIN64) || defined(_WIN32))
//	// Sizes
//    int inLen = strlen(strIn.c_str());
//    int wcInLen  = inLen + 1;
//
//	// In in wchar_t* format
//    wchar_t* wcIn = 0;
//    wcIn = new wchar_t[wcInLen];
//    memset(wcIn, 0, sizeof(wchar_t)*wcInLen);
//
//	// Conversion
//	MultiByteToWideChar(CP_UTF8, 0, strIn.c_str(), inLen, wcIn,wcInLen);
//
//	// In in wstring format
//	wstring wStrIn(strIn.length(), L' ');
//	wstringstream wss;
//	wss<<wcIn;
//	wStrIn = wss.str();
//
//	// Passing In from wstring format to string format
//	string strOut(wStrIn.length(), L' ');
//	std::copy(wStrIn.begin(), wStrIn.end(), strOut.begin());
//
//	return strOut;
//#else
//	return strIn;
//#endif
//}

string StringUtils::getStringValue(string file_name, string stringkey)
{
	list<string> keyvalues = FileHandler::getFileLines(file_name);
	
	string key;
	string value = stringkey;
	
	// Search given key in keyvalues
    while (!keyvalues.empty())
    {
		list<string> pair = split(keyvalues.front(), "=");

		// If key is the given key
		if (pair.size() == 2 && pair.front() == stringkey)
		{
			pair.pop_front();

			// Value is in this key-value pair
			value = pair.front();
			break;
		}
		
		keyvalues.pop_front();
    }
	
	return value;
}

list<string> StringUtils::split(string stringToSplit, const char* delimiter)
{
	int len;
	string strPart;
	list<string> allParts;
	
	char* buff = (char*)malloc(stringToSplit.length());
	buff = strstr((char*)stringToSplit.c_str(), delimiter);
	while(buff != NULL)
	{
		// Gets part of the string
		len = buff - stringToSplit.c_str();
		strPart = stringToSplit.substr(0, len);

		// Put it into list
		allParts.push_back(strPart);

		// Continue in string
		buff++;
		stringToSplit = string(buff);
		buff = strstr((char*)stringToSplit.c_str(), delimiter);
	}
	if (stringToSplit != "")
		allParts.push_back(stringToSplit);

	return allParts;
}
