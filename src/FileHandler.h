#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "StringUtils.h"
#include <list>
#include <string>

using namespace std;

class FileHandler
{
public:
	// Get a file buffer
	static unsigned char* getFileBuffer(string file_name, unsigned long* buffersize);

	// Get a file lines (file must have an empty line at the end because, on parsing, it finds extra weird characters in the end)
	static list<string> getFileLines(string path);
};

#endif // FILEHANDLER_H