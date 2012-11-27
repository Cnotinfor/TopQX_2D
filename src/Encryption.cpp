#include "Encryption.h"
#include "EfAes.H"
#include <sstream>
#include <locale>

using namespace std;

unsigned char Encryption::_key[16]={
    0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,
    0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88
};
unsigned char Encryption::_vector[16]={
    0x1f,0x32,0x43,0x51,0x56,0x98,0xaf,0xed,
    0xab,0xc8,0x21,0x45,0x63,0x72,0xac,0xfc
};

string Encryption::encode(string strToEncode)
{
	// Transform the string to a unsigned char buffer
	unsigned char inBuff[4096];
	memset(inBuff,0,4096);
	strcpy((char*)inBuff, strToEncode.c_str());

	// Encrypt
	unsigned char outBuff[4096];
	memset(outBuff,0,4096);
    AesCtx context;
    AesSetKey(&context, AES_KEY_128BIT, BLOCKMODE_CRT, _key , _vector);
	AesEncryptCRT(&context, outBuff, inBuff, strToEncode.length());

	// Transform output unsigned char buffer in string
	stringstream strStream;
	strStream<<outBuff;

	return strStream.str();
}

string Encryption::decode(string strToDecode)
{
	// Transform the string to a unsigned char buffer
	unsigned char inBuff[4096];
	memset(inBuff,0,4096);
	strcpy((char*)inBuff, strToDecode.c_str());
	
	// Decrypt
	unsigned char outBuff[4096];
	memset(outBuff,0,4096);
    AesCtx context;
    AesSetKey(&context, AES_KEY_128BIT, BLOCKMODE_CRT, _key , _vector);
	AesDecryptCRT(&context, outBuff, inBuff, strToDecode.length());

	// Transform output unsigned char buffer in string
	stringstream strStream;
	strStream<<outBuff;

	return strStream.str();
}