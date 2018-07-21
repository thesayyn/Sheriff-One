#pragma once
#include <string>
#include "cryptopp\cryptlib.h"
#include "cryptopp\filters.h"
#include "cryptopp\files.h"
#include "cryptopp\modes.h"
#include "cryptopp\queue.h"
#include "cryptopp\hex.h"
#include "cryptopp\aes.h"
#include <Windows.h>
#include <vector>

namespace Sheriff {

	class AES
	{
	public:
		static std::string iv;
		static std::string key;
		static std::string encrypt(std::string value);
		static std::vector<byte> encrypt(std::vector<byte> value);

	};
}

