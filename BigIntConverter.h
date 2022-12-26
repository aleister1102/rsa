#pragma once
#include "BigInt.h"
using namespace std;

class BigIntConverter
{
public:
	string reverseString(string str);
	string removeSpaces(string str);
	byte stringToByte(string str);
	string byteToString(byte number, bool isReversed = false);
	BigInt binaryStrToBigInt(string str);
	string bigIntToBinaryStr(BigInt number);
};

static BigIntConverter converter;