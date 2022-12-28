#pragma once
#include "BigInt.h"

using std::string;

class BigIntConverter
{
public:
	char intToDigit(int value);
	int digitToInt(char digit);

	byte stringToByte(string str);
	string byteToString(byte number, bool isReversed = false);

	BigInt binaryStrToBigInt(string str);
	string bigIntToBinaryStr(BigInt number);

	string bigIntToDecimalStr(BigInt n);
	BigInt decimalStrToBigInt(string str);
};

static BigIntConverter converter;