#include "stdafx.h"
#include "AES.h"



std::string Sheriff::AES::iv;
std::string Sheriff::AES::key;

std::string Sheriff::AES::encrypt(std::string value)
{
	byte bkey[CryptoPP::AES::MAX_KEYLENGTH];
	byte biv[CryptoPP::AES::BLOCKSIZE];


	if (CryptoPP::AES::MAX_KEYLENGTH < key.size())
		key = key.substr(0, CryptoPP::AES::MAX_KEYLENGTH);
	else if (CryptoPP::AES::MAX_KEYLENGTH > key.size())
		key += std::string(CryptoPP::AES::MAX_KEYLENGTH - key.size(), '*');

	if (CryptoPP::AES::BLOCKSIZE < iv.size())
		iv = iv.substr(0, CryptoPP::AES::BLOCKSIZE);
	else if (CryptoPP::AES::BLOCKSIZE > iv.size())
		iv += std::string(CryptoPP::AES::BLOCKSIZE - iv.size(), '*');

	memcpy(bkey, key.c_str(), CryptoPP::AES::MAX_KEYLENGTH);
	memcpy(biv, iv.c_str(), CryptoPP::AES::BLOCKSIZE);

	std::string aftertext;

	CryptoPP::AES::Encryption aesEncryption(bkey, CryptoPP::AES::MAX_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, biv);

	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(aftertext));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(value.c_str()), value.length() + 1);
	stfEncryptor.MessageEnd();

	return aftertext;
}


std::vector<byte> Sheriff::AES::encrypt(std::vector<byte> value)
{
	byte bkey[CryptoPP::AES::MAX_KEYLENGTH];
	byte biv[CryptoPP::AES::BLOCKSIZE];


	if (CryptoPP::AES::MAX_KEYLENGTH < key.size())
		key = key.substr(0, CryptoPP::AES::MAX_KEYLENGTH);
	else if (CryptoPP::AES::MAX_KEYLENGTH > key.size())
		key += std::string(CryptoPP::AES::MAX_KEYLENGTH - key.size(), '*');

	if (CryptoPP::AES::BLOCKSIZE < iv.size())
		iv = iv.substr(0, CryptoPP::AES::BLOCKSIZE);
	else if (CryptoPP::AES::BLOCKSIZE > iv.size())
		iv += std::string(CryptoPP::AES::BLOCKSIZE - iv.size(), '*');

	memcpy(bkey, key.c_str(), CryptoPP::AES::MAX_KEYLENGTH);
	memcpy(biv, iv.c_str(), CryptoPP::AES::BLOCKSIZE);

	std::vector<byte> result;

	CryptoPP::AES::Encryption aesEncryption(bkey, CryptoPP::AES::MAX_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, biv);

	result.resize(value.size() + CryptoPP::AES::BLOCKSIZE);

	CryptoPP::ArraySink cs(&result[0], result.size());

	CryptoPP::ArraySource(value.data(), value.size(), true,
		new CryptoPP::StreamTransformationFilter(cbcEncryption, new CryptoPP::Redirector(cs)));

	result.resize(cs.TotalPutLength());
	return result;
}

