#pragma once
#include "cryptopp/aes.h"

using namespace CryptoPP;

class Crpyt
{
public:
	static void Init(byte key[256]);
	static void Encrpyt(std::string text);
private:
	byte key[256];
	byte iv[AES::BLOCKSIZE];

};

