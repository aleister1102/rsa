#pragma once
#include "BigInt.h"

using std::string;
using std::tuple;

class Converter
{
public:
	char intToDigit(int value);
	int digitToInt(char digit);

	byte stringToByte(string str);
	string byteToString(byte number, bool isReversed = true);

	BigInt binaryStrToBigInt(string str);
	string bigIntToBinaryStr(BigInt number);

	string bigIntToDecimalStr(BigInt n);
	BigInt decimalStrToBigInt(string str);

	tuple<BigInt, BigInt, BigInt> toRSAKeys(tuple<string, string, string> keysStr, int base);
};

static Converter converter;