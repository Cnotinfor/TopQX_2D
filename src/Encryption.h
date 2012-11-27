#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

using namespace std;

class Encryption
{
public:
	// Encode string
	static string encode(string strToEncode);

	// Decode encoded string
	static string decode(string strToDecode);
private:
	static unsigned char _key[16];
	static unsigned char _vector[16];
};

#endif // ENCRYPTION_H